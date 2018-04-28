#include "NodeConnectionGraphicsItem.h"

#include "model/NodeConnection.h"
#include "model/NodePortModel.h"

#include "EditorColorScheme.h"

#include <QPen>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsScene>

#include <assert.h>

NodeConnectionGraphicsItem::NodeConnectionGraphicsItem()
{
	setAcceptHoverEvents(true);
	setFlags(ItemIsSelectable);
}

NodeConnectionGraphicsItem::~NodeConnectionGraphicsItem()
{
	assert(m_first_port != nullptr);
	assert(m_second_port != nullptr);
	m_first_port->remove_port_position_listener(this);
	m_second_port->remove_port_position_listener(this);
}

void NodeConnectionGraphicsItem::set_first_port(NodePortGraphicsItem* port)
{
	m_first_port = port;

	m_start_pos = port->scenePos() + OFFSET;
	m_end_pos = m_start_pos;
	if (scene())
		scene()->update();

}

void NodeConnectionGraphicsItem::update_line(const QPointF& end_pos)
{
	assert(m_first_port != nullptr);
	m_start_pos = m_first_port->scenePos() + OFFSET;
	m_end_pos = end_pos;
	if (scene())
		scene()->update();

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

	m_start_pos = m_first_port->scenePos() + OFFSET;
	m_end_pos = m_second_port->scenePos() + OFFSET;
	if (scene())
		scene()->update();
	update();
}

void NodeConnectionGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QPen pen;
	if (m_hover)
		pen.setWidth(2);
	else
		pen.setWidth(1);

	if (isSelected())
		pen.setColor(EditorColorScheme::selectedColor);
	else
		pen.setColor(EditorColorScheme::labelSublineColor_);

	painter->setPen(pen);
	QPainterPath path;

	QPointF start_pos;
	QPointF end_pos;
	QPointF ctrl1;
	QPointF ctrl2;
	if (m_first_port->port_model()->port_type() == NodePortModel::INPUT)
	{
		start_pos = m_start_pos;
		end_pos = m_end_pos;
		ctrl1 = start_pos - QPointF(80, 0);
		ctrl2 = end_pos + QPointF(80, 0);
	}
	else
	{
		start_pos = m_end_pos;
		end_pos = m_start_pos;
		ctrl1 = start_pos - QPointF(80, 0);
		ctrl2 = end_pos + QPointF(80, 0);
	}

	path.moveTo(start_pos);
	path.cubicTo(ctrl1, ctrl2, end_pos);

	painter->drawPath(path);

	// TODO debug troep goed krijgen
	//painter->drawPath(shape());
	//painter->drawRect(boundingRect());
}

QRectF NodeConnectionGraphicsItem::boundingRect() const
{
	return QRectF(m_start_pos + QPointF(20, 0), m_end_pos - QPointF(20, 0));
}

QPainterPath NodeConnectionGraphicsItem::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());
	path.closeSubpath();
	return path;
}

void NodeConnectionGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	Q_UNUSED(event);
	m_hover = true;
	scene()->update();
	update();
}

void NodeConnectionGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	Q_UNUSED(event);
	m_hover = false;
	scene()->update();
	update();
}

QVariant NodeConnectionGraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == ItemSelectedChange)
	{
		if (scene())
			scene()->update();
		update();
	}

	return QGraphicsItem::itemChange(change, value);
}

void NodeConnectionGraphicsItem::self_destruct()
{
	delete this;
}

void NodeConnectionGraphicsItem::set_connection(NodeConnection* connection)
{
	m_connection = connection;
	connect(connection, SIGNAL(connection_destroyed()), this, SLOT(self_destruct()));
}

NodeConnection* NodeConnectionGraphicsItem::connection()
{
	return m_connection;
}