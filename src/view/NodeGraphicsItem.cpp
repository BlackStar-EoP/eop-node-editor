#include "NodeGraphicsItem.h"

#include "NodePortGraphicsItem.h"

#include "model/NodePortModel.h"
#include "model/NodeModel.h"

#include <QPainter>
#include <QGraphicsTextItem>

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

void NodeGraphicsItem::initUI()
{
	QString name = m_node_model.title();
	if (m_node_model.is_orphan())
	name += " (Orphan)";

	QGraphicsTextItem* title = new QGraphicsTextItem(name, this);

	uint32_t num_input_ports = m_node_model.num_input_ports();
	for (uint32_t i = 0; i < num_input_ports; ++i)
	{
		NodePortModel* port_model = m_node_model.input_port_model(i);
		NodePortGraphicsItem* port_item = new NodePortGraphicsItem(this, *port_model, i);
	}

	uint32_t num_output_ports = m_node_model.num_output_ports();
	for (uint32_t i = 0; i < num_output_ports; ++i)
	{
		NodePortModel* port_model = m_node_model.output_port_model(i);
		NodePortGraphicsItem* port_item = new NodePortGraphicsItem(this, *port_model, i);
	}

	recalculate_size();
}

QRectF NodeGraphicsItem::boundingRect() const
{
	return m_bounding_rect;
}

void NodeGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->drawRoundedRect(m_bounding_rect, 10.0f, 10.0f);
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
	auto kak = childItems().size();
	qDeleteAll(childItems());
	initUI();
}

void NodeGraphicsItem::self_destruct()
{
	delete this;
}