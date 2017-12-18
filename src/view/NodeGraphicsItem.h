#pragma once

#include <QGraphicsItem>

class NodeGraphicsItem : public QGraphicsItem
{
public:
	NodeGraphicsItem(QGraphicsItem* parent);
	NodeGraphicsItem();

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
	QRectF m_bounding_rect = QRect(0, 0, 100, 50);
};