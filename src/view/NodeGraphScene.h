#pragma once

#include<QGraphicsScene>

class NodeGraphScene : public QGraphicsScene
{
public:
	NodeGraphScene(QObject* parent);

private:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};