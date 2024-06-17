#include "NodePortGraphicsItem.h"

#include "model/NodePortModel.h"

#include "EditorColorScheme.h"
#include "NodeGraphicsItem.h"
#include <QPainter>
#include <QFontMetrics>
#include <QGraphicsProxyWidget>
#include <QWidget>

#include <assert.h>

NodePortGraphicsItem::NodePortGraphicsItem(QGraphicsItem* parent, NodePortModel* port_model, uint32_t port_index)
: QGraphicsItem(parent)
, m_port_model(port_model)
, m_port_index(port_index)
{
//	m_parent = dynamic_cast<NodeGraphicsItem*>(parent);
	setFlags(ItemIsSelectable | ItemSendsScenePositionChanges);

	if (m_port_model->widget() != nullptr)
	{
		QGraphicsProxyWidget* proxy_widget = new QGraphicsProxyWidget(this);
		proxy_widget->setWidget(m_port_model->widget());
		//proxy_widget->setPos(QPointF(10, m_port_start_y));
		//m_port_start_y += m_node_model->widget()->height();
	}

	connect(port_model, SIGNAL(node_port_model_destroyed()), this, SLOT(selfdestruct()));
}

NodePortGraphicsItem::~NodePortGraphicsItem()
{
}

QRectF NodePortGraphicsItem::boundingRect() const
{
	return m_bounding_rect;
}

void NodePortGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	painter->setPen(EditorColorScheme::gridMajorColor_);
	if (m_port_model->num_connections() > 0)
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
	switch (m_port_model->port_type())
	{
	case NodePortModel::INPUT:
		text_pos += QPointF(30, 0);
		break;

	case NodePortModel::OUTPUT:
	{
		QFontMetrics fm = QFontMetrics(painter->font());
		QRect text_rect = fm.boundingRect(m_port_model->port_label());
		text_pos += QPointF(-text_rect.width() - 10, 0);

		if (m_port_model->widget() != nullptr)
		{
			text_pos -= QPointF(m_port_model->widget()->width(), 0);

			int x = text_pos.x() + text_rect.width() + 5;
			int y = 0;
			int w = m_port_model->widget()->width();
			int h = m_port_model->widget()->height();
			m_port_model->widget()->setGeometry(x, y, w, h);
		}

	}
	break;

	default:
		assert(false);
		break;
	}
	painter->drawText(text_pos, m_port_model->port_label());
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

NodePortModel* NodePortGraphicsItem::port_model() const
{
	return m_port_model;
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