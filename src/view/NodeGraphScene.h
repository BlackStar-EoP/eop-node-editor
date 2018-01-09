#pragma once

#include<QGraphicsScene>

class NodeGraphController;

class NodeGraphScene : public QGraphicsScene
{
public:
	NodeGraphScene(QObject* parent, NodeGraphController& controller);

private:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
	NodeGraphController& m_controller;

	QGraphicsLineItem* m_line_edit_item = nullptr;
	QPointF m_line_start_pos;
};