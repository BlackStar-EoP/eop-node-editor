#include "NodePortGraphicsItem.h"

#include "NodeGraphicsItem.h"

#include <QPainter>

NodePortGraphicsItem::NodePortGraphicsItem(QGraphicsItem* parent)
: QGraphicsItem(parent)
{
	setFlags(ItemIsMovable | ItemIsSelectable);
}

QRectF NodePortGraphicsItem::boundingRect() const
{
	return m_bounding_rect;
}

void NodePortGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->drawEllipse(pos(), 10, 10);
	painter->drawText(pos(), "Port test");
}