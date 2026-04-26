#include "NodeGraphicsItem.h"

#include "EditorColorScheme.h"
#include "NodePortWidget.h"

#include "model/NodePortModel.h"
#include "model/NodeModel.h"

#include <QPainter>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>
#include <QWidget>
#include <QBoxLayout>
#include <QLabel>

#include <assert.h>

class NodeModel;

namespace
{
constexpr float DEFAULT_WIDTH = 200;
constexpr float MARGIN_HEIGHT = 20.0f;
constexpr float MARGIN_WIDTH = 10.0f;

class ProxyWidgetSizeFilter : public QObject {
public:
    explicit ProxyWidgetSizeFilter(QObject *parent, std::function<void()> callback)
        : QObject(parent), m_callback(std::move(callback)) {}

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override {
        switch (ev->type()) {
        case QEvent::LayoutRequest:
        case QEvent::Resize:
            if (m_callback)
                m_callback();
            break;
        default:
            break;
        }
        return QObject::eventFilter(obj, ev);
    }

private:
    std::function<void()> m_callback;
};

}

NodeGraphicsItem::NodeGraphicsItem(NodeModel* node_model)
: m_node_model(node_model)
{
	setPos(node_model->position());
	setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
	initUI();
	setAcceptHoverEvents(true);
	connect(node_model, SIGNAL(node_model_destroyed()), this, SLOT(self_destruct()));
	connect(&EditorColorScheme::instance(), &EditorColorScheme::colorsChanged, this, [this]() {
		update();
	});
}

NodeGraphicsItem::~NodeGraphicsItem()
{
    m_node_model->unregister_node_model_listener(this);
}

QRectF NodeGraphicsItem::boundingRect() const
{
	return m_bounding_rect;
}

void NodeGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	QPen pen;
	if (m_hover)
		pen.setWidth(2);
	else
		pen.setWidth(1);

	painter->setBrush(QBrush(EditorColorScheme::rulerBackgroundColor()));
	if (isSelected())
		pen.setColor(EditorColorScheme::selectedColor());
	else
		pen.setColor(EditorColorScheme::labelColor());

	painter->setPen(pen);
	painter->drawRoundedRect(m_bounding_rect, 10.0f, 10.0f);
}

void NodeGraphicsItem::initUI()
{
	QString name = m_node_model->title();
	if (m_node_model->is_orphan())
		name += " (Orphan)";

    m_contents = new QWidget();
    m_layout = new QVBoxLayout(m_contents);
    m_layout->setSpacing(2);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addWidget(new QLabel(name));

	if (m_node_model->widget() != nullptr)
	{
        m_layout->addWidget(m_node_model->widget());
	}
    m_contents->installEventFilter(
            new ProxyWidgetSizeFilter(m_contents, [this]
                {
                    if (recalculate_size())
                    {
                        update_node_positions();
                        update();
                    }
                })
            );

    QGraphicsProxyWidget* proxy_widget = new QGraphicsProxyWidget(this);
    proxy_widget->setWidget(m_contents);
    proxy_widget->setPos(QPointF(MARGIN_WIDTH, MARGIN_HEIGHT));

    m_input_port_layout = new QVBoxLayout();
    m_layout->addLayout(m_input_port_layout);
    m_output_port_layout = new QVBoxLayout();
    m_layout->addLayout(m_output_port_layout);

	init_input_ports();
	init_output_ports();

	recalculate_size();

    update_node_positions();
}

void NodeGraphicsItem::init_input_ports()
{
	uint32_t num_input_ports = m_node_model->num_input_ports();
	for (uint32_t i = 0; i < num_input_ports; ++i)
	{
		NodePortModel* port_model = m_node_model->input_port_model(i);
        m_input_port_layout->addWidget(new NodePortWidget(port_model, i));
	}
}

void NodeGraphicsItem::init_output_ports()
{
	uint32_t num_output_ports = m_node_model->num_output_ports();
	for (uint32_t i = 0; i < num_output_ports; ++i)
	{
		NodePortModel* port_model = m_node_model->output_port_model(i);
        m_output_port_layout->addWidget(new NodePortWidget(port_model, i));
	}
}

bool NodeGraphicsItem::recalculate_size()
{
	float width = DEFAULT_WIDTH;
	float widgetHeight = 0;
	if (m_contents != nullptr)
	{
        m_contents->ensurePolished();
        m_contents->adjustSize();
        QSize widgetSize = m_contents->size();

		widgetHeight = widgetSize.height();
		width = widgetSize.width();
		width += 20;
	}

    QRectF old_bounding_rect = m_bounding_rect;
	m_bounding_rect = QRectF(0, 0, width, 2 * MARGIN_HEIGHT + widgetHeight);
    return old_bounding_rect != m_bounding_rect;
}

QVariant NodeGraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == ItemPositionHasChanged)
	{
		m_node_model->set_position(pos());
        m_node_model->update_port_models();
	}

	return QGraphicsItem::itemChange(change, value);
}

void NodeGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	Q_UNUSED(event);
	m_hover = true;
	update();
}

void NodeGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	Q_UNUSED(event);
	m_hover = false;
	update();
}

void NodeGraphicsItem::node_model_changed()
{
    remove_input_nodes();
    remove_output_nodes();
	init_input_ports();
	init_output_ports();
	recalculate_size();
    update_node_positions();

	update();
}

void NodeGraphicsItem::input_nodes_changed()
{
    remove_input_nodes();
    init_input_ports();
    recalculate_size();
    update_node_positions();
    update();
}

void NodeGraphicsItem::output_nodes_changed()
{
    remove_output_nodes();
	init_output_ports();
	recalculate_size();
    update_node_positions();
	update();
}

void NodeGraphicsItem::remove_input_nodes()
{
    while (QLayoutItem* item = m_input_port_layout->takeAt(0))
    {
        if (QWidget* widget = item->widget())
        {
            widget->setParent(nullptr);
            widget->deleteLater();
        }
        delete item;
    }
}

void NodeGraphicsItem::remove_output_nodes()
{
    while (QLayoutItem* item = m_output_port_layout->takeAt(0))
    {
        if (QWidget* widget = item->widget())
        {
            widget->setParent(nullptr);
            widget->deleteLater();
        }
        delete item;
    }
}

void NodeGraphicsItem::update_node_positions()
{
    m_node_model->update_port_models();
}

void NodeGraphicsItem::self_destruct()
{
	delete this;
}

NodeModel* NodeGraphicsItem::node_model()
{
	return m_node_model;
}