#include "NodeGraphScene.h"

#include "NodeGraphicsItem.h"

#include "controllers/NodeGraphController.h"
#include "model/Node.h"

#include <QGraphicsSceneMouseEvent>

NodeGraphScene::NodeGraphScene(QObject* parent, NodeGraphController& controller)
: QGraphicsScene(parent)
, m_controller(controller)
{
}

void NodeGraphScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
	if (item == nullptr)
	{
		Node* node = m_controller.add_node(event->scenePos());
		NodeGraphicsItem* item = new NodeGraphicsItem(*node);
		addItem(item);
	}
	else
	{
		QGraphicsScene::mousePressEvent(event);
	}
}