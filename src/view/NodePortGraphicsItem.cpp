#include "NodePortGraphicsItem.h"

#include "model/NodePort.h"

#include "NodeGraphicsItem.h"
#include <QPainter>

#include <assert.h>

NodePortGraphicsItem::NodePortGraphicsItem(QGraphicsItem* parent, const NodePort& node_port, uint32_t port_index)
: QGraphicsItem(parent)
, m_node_port(node_port)
, m_port_index(port_index)
{
	setFlags(ItemIsMovable | ItemIsSelectable);

	set_port_position();
}

QRectF NodePortGraphicsItem::boundingRect() const
{
	return m_bounding_rect;
}

void NodePortGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->drawEllipse(pos(), 10, 10);
	QPointF text_pos = pos();
	switch (m_node_port.port_type())
	{
	case NodePort::INPUT:
		text_pos += QPointF(20, 0);
		break;

	case NodePort::OUTPUT:
		text_pos += QPointF(-50, 0);
		break;

	default:
		assert(false);
		break;
	}
	painter->drawText(text_pos, m_node_port.port_label());
}

void NodePortGraphicsItem::set_port_position()
{
	switch (m_node_port.port_type())
	{
	case NodePort::INPUT:
		setPos(10, 10 + (10 * m_port_index));
		break;

	case NodePort::OUTPUT:
		setPos(30, 10 + (10 * m_port_index));
		break;

	default:
		assert(false);
		break;
	}
}