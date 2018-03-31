#include "NodeGraphicsItem.h"

#include "NodePortGraphicsItem.h"
#include "EditorColorScheme.h"

#include "model/NodePortModel.h"
#include "model/NodeModel.h"

#include <QPainter>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>

#include <assert.h>

class NodeModel;

NodeGraphicsItem::NodeGraphicsItem(NodeModel& node_model)
: m_node_model(node_model)
{
	setPos(node_model.position());
	setFlags(ItemIsMovable | ItemIsSelectable);

	initUI();

	setFlag(QGraphicsItem::ItemSendsGeometryChanges);

	connect(&node_model, SIGNAL(node_model_destroyed()), this, SLOT(self_destruct()));
}

QRectF NodeGraphicsItem::boundingRect() const
{
	return m_bounding_rect;
}

void NodeGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setBrush(QBrush(EditorColorScheme::rulerBackgroundColor_));
	painter->setPen(EditorColorScheme::labelColor_);

	painter->drawRoundedRect(m_bounding_rect, 10.0f, 10.0f);
}

void NodeGraphicsItem::initUI()
{
	QString name = m_node_model.title();
	if (m_node_model.is_orphan())
		name += " (Orphan)";

	QGraphicsTextItem* title = new QGraphicsTextItem(name, this);
	title->setDefaultTextColor(EditorColorScheme::labelTitleColor_);

	if (m_node_model.widget() != nullptr)
	{
		QGraphicsProxyWidget* proxy_widget = new QGraphicsProxyWidget(this);
		proxy_widget->setWidget(m_node_model.widget());
	}

	init_input_ports();
	init_output_ports();

	recalculate_size();
}

void NodeGraphicsItem::init_input_ports()
{
	uint32_t num_input_ports = m_node_model.num_input_ports();
	for (uint32_t i = 0; i < num_input_ports; ++i)
	{
		NodePortModel* port_model = m_node_model.input_port_model(i);
		NodePortGraphicsItem* port_item = new NodePortGraphicsItem(this, *port_model, i);
		m_input_ports.push_back(port_item);
	}
}

void NodeGraphicsItem::init_output_ports()
{
	uint32_t num_output_ports = m_node_model.num_output_ports();
	for (uint32_t i = 0; i < num_output_ports; ++i)
	{
		NodePortModel* port_model = m_node_model.output_port_model(i);
		NodePortGraphicsItem* port_item = new NodePortGraphicsItem(this, *port_model, i);
		m_output_ports.push_back(port_item);
	}
}


void NodeGraphicsItem::recalculate_size()
{
	const uint32_t WIDTH = 200;
	const uint32_t HEIGHT = 50;
	const uint32_t PORT_HEIGHT = 25;

	uint32_t port_height = PORT_HEIGHT * m_node_model.num_ports();
	m_bounding_rect = QRectF(0, 0, WIDTH, port_height + HEIGHT);
}

QVariant NodeGraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == ItemPositionHasChanged)
	{
		m_node_model.set_position(pos());
	}

	return QGraphicsItem::itemChange(change, value);
}

void NodeGraphicsItem::node_model_changed()
{
	assert(false);
}

void NodeGraphicsItem::output_nodes_changed()
{
	// This works because the output port items are last. if input changes or node model needs to change for whatever reason, 
	// this solution is not adequate, so, basically a TODO if ever.
	qDeleteAll(m_output_ports);
	init_output_ports();
}

void NodeGraphicsItem::self_destruct()
{
	delete this;
}