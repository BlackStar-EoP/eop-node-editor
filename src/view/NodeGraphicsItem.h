#pragma once

#include <src/model/NodeModel.h>
#include <QGraphicsItem>

class Node;

class NodeGraphicsItem : public QGraphicsItem, public INodeModelListener
{
public:
	NodeGraphicsItem(Node& node);
	void initUI();

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
	void recalculate_size();
	QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
	virtual void node_model_changed() override;

private:
	QRectF m_bounding_rect = QRect(0, 0, 100, 50);
	Node& m_node;
};