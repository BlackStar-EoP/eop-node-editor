#include "NodeGraphicsItem.h"

#include "NodePortGraphicsItem.h"
#include "EditorColorScheme.h"

#include "model/NodePortModel.h"
#include "model/NodeModel.h"

#include <QPainter>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>
#include <QWidget>

#include <assert.h>

class NodeModel;

NodeGraphicsItem::NodeGraphicsItem(NodeModel* node_model)
: m_node_model(node_model)
{
	setPos(node_model->position());
	setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
	initUI();
	setAcceptHoverEvents(true);
	connect(node_model, SIGNAL(node_model_destroyed()), this, SLOT(self_destruct()));
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
	QPen pen;
	if (m_hover)
		pen.setWidth(2);
	else
		pen.setWidth(1);

	painter->setBrush(QBrush(EditorColorScheme::rulerBackgroundColor_));
	if (isSelected())
		pen.setColor(EditorColorScheme::selectedColor);
	else
		pen.setColor(EditorColorScheme::labelColor_);

	painter->setPen(pen);
	painter->drawRoundedRect(m_bounding_rect, 10.0f, 10.0f);
}

void NodeGraphicsItem::initUI()
{
	recalculate_size();
	m_port_start_y = 20;

	QString name = m_node_model->title();
	if (m_node_model->is_orphan())
		name += " (Orphan)";

	QGraphicsTextItem* title = new QGraphicsTextItem(name, this);
	title->setDefaultTextColor(EditorColorScheme::labelTitleColor_);
	title->setPos(QPointF(10, m_port_start_y));
	m_port_start_y += 25;

	if (m_node_model->widget() != nullptr)
	{
		QGraphicsProxyWidget* proxy_widget = new QGraphicsProxyWidget(this);
		proxy_widget->setWidget(m_node_model->widget());
		proxy_widget->setPos(QPointF(10, m_port_start_y));
		m_port_start_y += m_node_model->widget()->height();
	}

	init_input_ports();
	init_output_ports();
}

void NodeGraphicsItem::init_input_ports()
{
	uint32_t current_port_y = m_port_start_y;
	uint32_t num_input_ports = m_node_model->num_input_ports();
	for (uint32_t i = 0; i < num_input_ports; ++i)
	{
		NodePortModel* port_model = m_node_model->input_port_model(i);
		NodePortGraphicsItem* port_item = new NodePortGraphicsItem(this, port_model, i);
		port_item->setPos(QPointF(10, current_port_y));
		current_port_y += 25;
		m_input_ports.push_back(port_item);
	}
}

void NodeGraphicsItem::init_output_ports()
{
	uint32_t current_port_y = m_port_start_y + (m_node_model->num_input_ports() * 25);
	uint32_t num_output_ports = m_node_model->num_output_ports();
	for (uint32_t i = 0; i < num_output_ports; ++i)
	{
		NodePortModel* port_model = m_node_model->output_port_model(i);
		NodePortGraphicsItem* port_item = new NodePortGraphicsItem(this, port_model, i);
		port_item->setPos(QPointF(m_bounding_rect.width() - 30, current_port_y)); // TODO why is this 30?
		current_port_y += 25;
		m_output_ports.push_back(port_item);
	}
}

void NodeGraphicsItem::recalculate_size()
{
	const uint32_t WIDTH = 200;
	const uint32_t HEIGHT = 80;
	const uint32_t PORT_HEIGHT = 25;

	uint32_t width = WIDTH;
	uint32_t widgetHeight = 0;
	if (m_node_model->widget() != nullptr)
	{
		widgetHeight = m_node_model->widget()->size().height();
		width = m_node_model->widget()->size().width();
		width += 20;
	}

	uint32_t port_height = PORT_HEIGHT * m_node_model->num_ports();
	m_bounding_rect = QRectF(0, 0, width, port_height + HEIGHT + widgetHeight);
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

	init_input_ports();
	init_output_ports();

	recalculate_size();
	update();
}

void NodeGraphicsItem::output_nodes_changed()
{
	// This works because the output port items are last. if input changes or node model needs to change for whatever reason, 
	// this solution is not adequate, so, basically a TODO if ever.
	//qDeleteAll(m_output_ports);
	m_output_ports.clear();
	init_output_ports();
	recalculate_size();
	update();
}

void NodeGraphicsItem::self_destruct()
{
	delete this;
}

NodeModel* NodeGraphicsItem::node_model()
{
	return m_node_model;
}