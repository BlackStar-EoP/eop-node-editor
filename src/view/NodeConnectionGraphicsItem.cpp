#include "NodeConnectionGraphicsItem.h"

#include "model/NodeConnection.h"
#include "model/NodePortModel.h"

#include "EditorColorScheme.h"
#include "NodePortConnectorWidget.h"

#include <QPen>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsScene>

#include <assert.h>

NodeConnectionGraphicsItem::NodeConnectionGraphicsItem()
{
	setAcceptHoverEvents(true);
	setFlags(ItemIsSelectable | ItemSendsGeometryChanges);
	connect(&EditorColorScheme::instance(), &EditorColorScheme::colorsChanged, this, [this]() {
		update();
	});
}

bool NodeConnectionGraphicsItem::add_port(NodePortConnectorWidget* port)
{
    assert(port != nullptr);
    if (port->port_model()->port_type() == NodePortModel::INPUT)
    {
        if (m_input_port != nullptr)
            return false;
        m_input_port = port;
        connect(m_input_port, SIGNAL(position_changed()), this, SLOT(update_port_positions()));
    }
    else
    {
        if (m_output_port != nullptr)
            return false;
        m_output_port = port;
        connect(m_output_port, SIGNAL(position_changed()), this, SLOT(update_port_positions()));
    }
    update_port_positions();
    return true;
}

void NodeConnectionGraphicsItem::update_line(const QPointF& end_pos)
{
    m_line_pos = end_pos;
    update_port_positions();
}

void NodeConnectionGraphicsItem::update_port_positions()
{
    prepareGeometryChange();

    if (m_input_port != nullptr)
    {
        m_input_pos = m_input_port->attachment_scene_pos();
    }

    if (m_output_port != nullptr)
    {
        m_output_pos = m_output_port->attachment_scene_pos();
    }

    if (m_input_port == nullptr)
    {
        m_input_pos = m_line_pos.value_or(m_output_pos);
    }
    if (m_output_port == nullptr)
    {
        m_output_pos = m_line_pos.value_or(m_input_pos);
    }

    setPos(m_output_pos);
    update_bounding_rect();

	if (scene() != nullptr)
    {
		scene()->update();
    }

	update();
}

void NodeConnectionGraphicsItem::update_bounding_rect()
{
    QPointF output_local_pos = QPointF(0, 0);
    QPointF input_local_pos = m_input_pos - m_output_pos;

    qreal x1 = std::min(input_local_pos.x(), output_local_pos.x());
    qreal y1 = std::min(input_local_pos.y(), output_local_pos.y());
    qreal x2 = std::max(input_local_pos.x(), output_local_pos.x());
    qreal y2 = std::max(input_local_pos.y(), output_local_pos.y());

    x1 -= 20;
    x2 += 20;
    y1 -= 20;
    y2 += 20;
    if (m_input_pos.x() < m_output_pos.x())
    {
        x1 -= 20;
        x2 += 20;
    }

    m_bounding_rect = QRectF(x1, y1, x2 - x1, y2 - y1);
}

void NodeConnectionGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	QPen pen;
	if (m_hover)
		pen.setWidth(2);
	else
		pen.setWidth(1);

	if (isSelected())
		pen.setColor(EditorColorScheme::selectedColor());
	else
		pen.setColor(EditorColorScheme::labelSublineColor());

	if (m_connection != nullptr &&!m_connection->is_valid())
		pen.setColor(EditorColorScheme::invalidColor());

	painter->setPen(pen);
	QPainterPath path = create_path();
	painter->drawPath(path);

#ifdef DEBUG_NODE_CONNECTION
    pen.setColor(QColorConstants::Red);
	painter->setPen(pen);
    painter->drawRect(boundingRect());
    pen.setColor(QColorConstants::Yellow);
	painter->setPen(pen);
    painter->drawPath(shape());
#endif
}

QRectF NodeConnectionGraphicsItem::boundingRect() const
{
	return m_bounding_rect;
}

QPainterPath NodeConnectionGraphicsItem::shape() const
{
    QPainterPathStroker stroker;
    stroker.setWidth(20);
    return stroker.createStroke(create_path());
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

QPainterPath NodeConnectionGraphicsItem::create_path() const
{
	QPainterPath path;

    QPointF local_input_pos = m_input_pos - pos();

    QPointF ctrl1 = QPointF(80, 0);
    QPointF ctrl2 = local_input_pos - QPointF(80, 0);

	path.cubicTo(ctrl1, ctrl2, local_input_pos);

    return path;
}