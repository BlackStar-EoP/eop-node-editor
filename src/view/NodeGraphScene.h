#pragma once

#include<QGraphicsScene>

class NodeGraphController;
class NodeConnectionGraphicsItem;

class NodeGraphScene : public QGraphicsScene
{
public:
	NodeGraphScene(QObject* parent, NodeGraphController& controller);

private:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* keyEvent) override;
	void drawBackground(QPainter* painter, const QRectF& rect) override;

private:
	NodeGraphController& m_controller;

	NodeConnectionGraphicsItem* m_line_edit_item = nullptr;
	QPointF m_line_start_pos;
};