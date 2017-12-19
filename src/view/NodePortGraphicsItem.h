#pragma once

#include <QGraphicsItem>

class NodePortGraphicsItem : public QGraphicsItem
{
public:
	NodePortGraphicsItem(QGraphicsItem* parent);

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
	QRectF m_bounding_rect = QRect(0, 0, 10, 10);
};