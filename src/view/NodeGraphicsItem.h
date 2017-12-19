#pragma once

#include <QGraphicsItem>

class Node;

class NodeGraphicsItem : public QGraphicsItem
{
public:
	NodeGraphicsItem(const Node& node);

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
	void recalculate_size();

private:
	QRectF m_bounding_rect = QRect(0, 0, 100, 50);
	const Node& m_node;
};