#pragma once

#include <QGraphicsItem>

class Node;

class NodeGraphicsItem : public QGraphicsItem
{
public:
	NodeGraphicsItem(Node& node);

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
	void recalculate_size();
	QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
	QRectF m_bounding_rect = QRect(0, 0, 100, 50);
	Node& m_node;
};