#include "NodeConnectionGraphicsItem.h"

#include "EditorColorScheme.h"

#include <QPen>

#include <assert.h>

NodeConnectionGraphicsItem::NodeConnectionGraphicsItem()
{
	setAcceptHoverEvents(true);
}

NodeConnectionGraphicsItem::~NodeConnectionGraphicsItem()
{
	assert(m_first_port != nullptr);
	assert(m_second_port != nullptr);
	//m_first_port->remove_port_position_listener(this);
	//m_second_port->remove_port_position_listener(this);
}

void NodeConnectionGraphicsItem::set_first_port(NodePortGraphicsItem* port)
{
	m_first_port = port;

	const QPointF& start_pos = port->scenePos() + OFFSET;
	setLine(QLineF(start_pos, start_pos));
}

void NodeConnectionGraphicsItem::update_line(const QPointF& end_pos)
{
	assert(m_first_port != nullptr);
	const QPointF& start_pos = m_first_port->scenePos() + OFFSET;
	setLine(QLineF(start_pos, end_pos));
}

void NodeConnectionGraphicsItem::set_second_port(NodePortGraphicsItem* port)
{
	m_second_port = port;
	portPositionChanged();

	m_first_port->add_port_position_listener(this);
	m_second_port->add_port_position_listener(this);
}

void NodeConnectionGraphicsItem::portPositionChanged()
{
	assert(m_first_port != nullptr);
	assert(m_second_port != nullptr);

	const QPointF& start_pos = m_first_port->scenePos() + OFFSET;
	const QPointF& end_pos = m_second_port->scenePos() + OFFSET;
	setLine(QLineF(start_pos, end_pos));
}

void NodeConnectionGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	Q_UNUSED(event);
	QPen pen;
	pen.setWidth(2);
	pen.setColor(EditorColorScheme::labelSublineColor_);
	setPen(pen);
}

void NodeConnectionGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	Q_UNUSED(event);
	QPen pen;
	pen.setWidth(1);
	pen.setColor(EditorColorScheme::labelSublineColor_);
	setPen(pen);
}

void NodeConnectionGraphicsItem::self_destruct()
{
	delete this;
}

void NodeConnectionGraphicsItem::set_connection(NodeConnection* connection)
{
	m_connection = connection;
}

NodeConnection* NodeConnectionGraphicsItem::connection()
{
	return m_connection;
}