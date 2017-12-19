#pragma once

#include<QGraphicsScene>

class NodeGraphController;

class NodeGraphScene : public QGraphicsScene
{
public:
	NodeGraphScene(QObject* parent, NodeGraphController& controller);

private:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
	NodeGraphController& m_controller;
};