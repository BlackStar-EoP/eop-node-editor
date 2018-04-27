#include "NodePortGraphicsItem.h"

#include "model/NodePortModel.h"

#include "EditorColorScheme.h"
#include "NodeGraphicsItem.h"
#include <QPainter>

#include <assert.h>

NodePortGraphicsItem::NodePortGraphicsItem(QGraphicsItem* parent, NodePortModel& node_port_model, uint32_t port_index)
: QGraphicsItem(parent)
, m_node_port_model(node_port_model)
, m_port_index(port_index)
{
	setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsScenePositionChanges);

	connect(&node_port_model, SIGNAL(node_port_model_destroyed()), this, SLOT(selfdestruct()));
}

QRectF NodePortGraphicsItem::boundingRect() const
{
	return m_bounding_rect;
}

void NodePortGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setPen(EditorColorScheme::gridMajorColor_);
	if (m_node_port_model.num_connections() > 0)
	{
		painter->setBrush(EditorColorScheme::connection_color);
		painter->drawEllipse(0, 0, 20, 20);
	}
	else
	{
		painter->drawEllipse(0, 0, 20, 20);
	}
	painter->setBrush(Qt::NoBrush);
	painter->setPen(EditorColorScheme::labelColor_);
//	painter->drawRect(m_bounding_rect);
	QPointF text_pos = QPointF(0, 15);
	switch (m_node_port_model.port_type())
	{
	case NodePortModel::INPUT:
		text_pos += QPointF(20, 0);
		break;

	case NodePortModel::OUTPUT:
		text_pos += QPointF(-50, 0);
		break;

	default:
		assert(false);
		break;
	}
	painter->drawText(text_pos, m_node_port_model.port_label());
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

NodePortModel& NodePortGraphicsItem::node_port_model() const
{
	return m_node_port_model;
}

void NodePortGraphicsItem::add_port_position_listener(PortPositionListener* listener)
{
	m_port_position_listeners.insert(listener);
}

void NodePortGraphicsItem::remove_port_position_listener(PortPositionListener* listener)
{
	m_port_position_listeners.erase(listener);
}

void NodePortGraphicsItem::notify_position_listeners()
{
	for (PortPositionListener* l : m_port_position_listeners)
		l->portPositionChanged();
}

void NodePortGraphicsItem::selfdestruct()
{
	delete this;
}