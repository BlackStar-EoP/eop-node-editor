#include "NodeGraphicsItem.h"

#include "NodePortGraphicsItem.h"
#include "EditorColorScheme.h"

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
constexpr float PORT_HEIGHT = 25.0f;
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

NodePortGraphicsItem* NodeGraphicsItem::find_input_port(NodePortModel* input_port)
{
	for (NodePortGraphicsItem* port_gfx_item : m_input_ports)
	{
		if (port_gfx_item->port_model() == input_port)
			return port_gfx_item;
	}

	assert(false);
	return nullptr;
}

NodePortGraphicsItem* NodeGraphicsItem::find_output_port(NodePortModel* output_port)
{
	for (NodePortGraphicsItem* port_gfx_item : m_output_ports)
	{
		if (port_gfx_item->port_model() == output_port)
			return port_gfx_item;
	}

	assert(false);
	return nullptr;
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
    QBoxLayout* layout = new QVBoxLayout(m_contents);
    layout->addWidget(new QLabel(name));

	if (m_node_model->widget() != nullptr)
	{
        layout->addWidget(m_node_model->widget());
	}
    m_contents->installEventFilter(
            new ProxyWidgetSizeFilter(m_contents, [this]
                {
                    if (recalculate_size())
                    {
                        update_node_positions();
                    }
                })
            );

    QGraphicsProxyWidget* proxy_widget = new QGraphicsProxyWidget(this);
    proxy_widget->setWidget(m_contents);
    proxy_widget->setPos(QPointF(MARGIN_WIDTH, MARGIN_HEIGHT));

	recalculate_size();

	init_input_ports();
	init_output_ports();
    update_node_positions();
}

void NodeGraphicsItem::init_input_ports()
{
	uint32_t num_input_ports = m_node_model->num_input_ports();
	for (uint32_t i = 0; i < num_input_ports; ++i)
	{
		NodePortModel* port_model = m_node_model->input_port_model(i);
		NodePortGraphicsItem* port_item = new NodePortGraphicsItem(this, port_model, i);
		m_input_ports.push_back(port_item);
	}
}

void NodeGraphicsItem::init_output_ports()
{
	uint32_t num_output_ports = m_node_model->num_output_ports();
	for (uint32_t i = 0; i < num_output_ports; ++i)
	{
		NodePortModel* port_model = m_node_model->output_port_model(i);
		NodePortGraphicsItem* port_item = new NodePortGraphicsItem(this, port_model, i);
		m_output_ports.push_back(port_item);
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

	const float port_height = PORT_HEIGHT * m_node_model->num_ports();

    QRectF old_bounding_rect = m_bounding_rect;
	m_bounding_rect = QRectF(0, 0, width, port_height + 2 * MARGIN_HEIGHT + widgetHeight);
    return old_bounding_rect != m_bounding_rect;
}

QVariant NodeGraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == ItemPositionHasChanged)
	{
		m_node_model->set_position(pos());
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
	//qDeleteAll(m_input_ports);
	m_input_ports.clear();

	//qDeleteAll(m_output_ports);
	m_output_ports.clear();

	recalculate_size();

	init_input_ports();
	init_output_ports();
    update_node_positions();

	update();
}

void NodeGraphicsItem::input_nodes_changed()
{
    //qDeleteAll(m_input_ports);
    m_input_ports.clear();
    recalculate_size();
    init_input_ports();
    update_node_positions();
    update();
}

void NodeGraphicsItem::output_nodes_changed()
{
	//qDeleteAll(m_output_ports);
	m_output_ports.clear();
	recalculate_size();
	init_output_ports();
    update_node_positions();
	update();
}

void NodeGraphicsItem::update_node_positions()
{
    const float input_port_start_y = MARGIN_HEIGHT + m_contents->size().height();
	const size_t num_input_ports = m_input_ports.size();
    for (size_t i = 0; i < num_input_ports; i++)
    {
        const float y = input_port_start_y + i * PORT_HEIGHT;
		m_input_ports[i]->setPos(QPointF(MARGIN_WIDTH, y));
    }

    const float output_port_start_y = MARGIN_HEIGHT + m_contents->size().height()
        + num_input_ports * PORT_HEIGHT;
	const size_t num_output_ports = m_output_ports.size();
	for (size_t i = 0; i < num_output_ports; ++i)
	{
        const float y = output_port_start_y + i * PORT_HEIGHT;
		m_output_ports[i]->setPos(QPointF(m_bounding_rect.width() - NodePortGraphicsItem::PORT_RADIUS - MARGIN_WIDTH, y));
	}
}

void NodeGraphicsItem::self_destruct()
{
	delete this;
}

NodeModel* NodeGraphicsItem::node_model()
{
	return m_node_model;
}