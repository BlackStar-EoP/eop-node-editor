#include "NodeGraphicsItem.h"

#include "NodePortGraphicsItem.h"

#include <QPainter>

NodeGraphicsItem::NodeGraphicsItem(QGraphicsItem* parent)
: QGraphicsItem(parent)
{
		//QPainterPath p;
		//p.addRoundedRect(-50, -15, 100, 30, 5, 5);
		//setPath(p);
		//setPen(QPen(Qt::darkGreen));
		//setBrush(Qt::green);
		//setFlag(QGraphicsItem::ItemIsMovable);
		//setFlag(QGraphicsItem::ItemIsSelectable);
		//horzMargin = 20;
		//vertMargin = 5;
		//width = horzMargin;
		//height = vertMargin;
	setFlags(ItemIsMovable | ItemIsSelectable);
}

NodeGraphicsItem::NodeGraphicsItem()
{
	setFlags(ItemIsMovable | ItemIsSelectable);
	NodePortGraphicsItem* inp = new NodePortGraphicsItem(this);
	inp->setPos(10, 10);

	NodePortGraphicsItem* outp = new NodePortGraphicsItem(this);
	outp->setPos(30, 20);
}

QRectF NodeGraphicsItem::boundingRect() const
{
	return m_bounding_rect;
}

void NodeGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->drawRoundedRect(m_bounding_rect, 10.0f, 10.0f);
}