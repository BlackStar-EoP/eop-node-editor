#include "NodeGraphicsItem.h"

#include "NodePortGraphicsItem.h"

#include "model/NodePort.h"
#include "model/Node.h"

#include <QPainter>

NodeGraphicsItem::NodeGraphicsItem(const Node& node)
: m_node(node)
{
	setPos(node.position());
	setFlags(ItemIsMovable | ItemIsSelectable);
	const QVector<NodePort>& input_ports = node.input_ports();
	const QVector<NodePort>& output_ports = node.output_ports();

	uint32_t port_index = 0;
	for (const NodePort& port : input_ports)
	{
		NodePortGraphicsItem* port_item = new NodePortGraphicsItem(this, port, port_index++);
	}

	for (const NodePort& port : output_ports)
	{
		NodePortGraphicsItem* port_item = new NodePortGraphicsItem(this, port, port_index++);
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
	const uint32_t PORT_HEIGHT = 20;

	uint32_t port_height = PORT_HEIGHT * m_node.num_ports();
	m_bounding_rect = QRectF(0, 0, WIDTH, port_height + HEIGHT);
}