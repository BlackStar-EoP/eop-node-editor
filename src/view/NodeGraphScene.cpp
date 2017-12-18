#include "NodeGraphScene.h"

#include "NodeGraphicsItem.h"

#include <QGraphicsSceneMouseEvent>

NodeGraphScene::NodeGraphScene(QObject* parent)
: QGraphicsScene(parent)
{
	addItem(new NodeGraphicsItem());
}

void NodeGraphScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
	if (item == nullptr)
	{
		NodeGraphicsItem* item = new NodeGraphicsItem();
		item->setPos(event->scenePos());
		addItem(item);
	}
	else
	{
		QGraphicsScene::mousePressEvent(event);
	}
}