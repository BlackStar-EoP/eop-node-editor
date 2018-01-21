#include "NodePortGraphicsItem.h"

#include "model/NodePort.h"

#include "NodeGraphicsItem.h"
#include <QPainter>

#include <assert.h>

NodePortGraphicsItem::NodePortGraphicsItem(QGraphicsItem* parent, NodePort& node_port, uint32_t port_index)
: QGraphicsItem(parent)
, m_node_port(node_port)
, m_port_index(port_index)
{
	set_port_position();
	setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsScenePositionChanges);
}

QRectF NodePortGraphicsItem::boundingRect() const
{
	return m_bounding_rect;
}

void NodePortGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	if (m_selected)
	{
		painter->setBrush(Qt::red);
		painter->drawEllipse(0, 0, 20, 20);
	}
	else
	{
		painter->drawEllipse(0, 0, 20, 20);
	}
	painter->setBrush(Qt::NoBrush);
	painter->setPen(Qt::black);
	painter->drawRect(m_bounding_rect);
	QPointF text_pos = QPointF(0, 15);
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

QVariant NodePortGraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == ItemScenePositionHasChanged)
	{
		notify_position_listeners();
	}

	return QGraphicsItem::itemChange(change, value);
}


void NodePortGraphicsItem::select()
{
	m_selected = !m_selected;
	update();
}

NodePort& NodePortGraphicsItem::node_port() const
{
	return m_node_port;
}

void NodePortGraphicsItem::add_port_position_listener(PortPositionListener* listener)
{
	m_port_position_listeners.insert(listener);
}

void NodePortGraphicsItem::remove_port_position_listener(PortPositionListener* listener)
{
	m_port_position_listeners.erase(listener);
}

void NodePortGraphicsItem::set_port_position()
{
	switch (m_node_port.port_type())
	{
	case NodePort::INPUT:
		setPos(10, 10 + (25 * m_port_index));
		break;

	case NodePort::OUTPUT:
		setPos(170, 10 + (25 * m_port_index));
		break;

	default:
		assert(false);
		break;
	}
}

void NodePortGraphicsItem::notify_position_listeners()
{
	for (PortPositionListener* l : m_port_position_listeners)
		l->portPositionChanged();
}