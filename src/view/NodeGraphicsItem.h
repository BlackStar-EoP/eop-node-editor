#pragma once

#include <QObject>
#include <src/model/NodeModel.h>
#include <QGraphicsItem>

class NodeGraphicsItem : public QObject, public QGraphicsItem, public INodeModelListener
{
	Q_OBJECT
public:
	NodeGraphicsItem(NodeModel& node_model);
	void initUI();

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
	void recalculate_size();
	QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
	virtual void node_model_changed() override;

private slots:
	void self_destruct();

private:
	QRectF m_bounding_rect = QRect(0, 0, 100, 50);
	NodeModel& m_node_model;
};