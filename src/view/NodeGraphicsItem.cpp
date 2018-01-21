#include "NodeGraphicsItem.h"

#include "NodePortGraphicsItem.h"

#include "model/NodePort.h"
#include "model/Node.h"

#include <QPainter>

NodeGraphicsItem::NodeGraphicsItem(Node& node)
: m_node(node)
{
	setPos(node.position());
	setFlags(ItemIsMovable | ItemIsSelectable);
	const QVector<NodePort*>& input_ports = node.input_ports();
	const QVector<NodePort*>& output_ports = node.output_ports();

	uint32_t port_index = 0;
	for (NodePort* port : input_ports)
	{
		NodePortGraphicsItem* port_item = new NodePortGraphicsItem(this, *port, port_index++);
	}

	for (NodePort* port : output_ports)
	{
		NodePortGraphicsItem* port_item = new NodePortGraphicsItem(this, *port, port_index++);
	}

	recalculate_size();

	setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF NodeGraphicsItem::boundingRect() const
{
	return m_bounding_rect;
}

void NodeGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->drawRoundedRect(m_bounding_rect, 10.0f, 10.0f);
	QString name = m_node.name();
	if (m_node.is_orphan())
		name += " (Orphan)";
	painter->drawText(QPoint(20, 20), name);
}

void NodeGraphicsItem::recalculate_size()
{
	const uint32_t WIDTH = 200;
	const uint32_t HEIGHT = 50;
	const uint32_t PORT_HEIGHT = 25;

	uint32_t port_height = PORT_HEIGHT * m_node.num_ports();
	m_bounding_rect = QRectF(0, 0, WIDTH, port_height + HEIGHT);
}

QVariant NodeGraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == ItemPositionHasChanged)
	{
		m_node.set_position(pos());
	}

	return QGraphicsItem::itemChange(change, value);
}