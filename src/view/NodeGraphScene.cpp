#include "NodeGraphScene.h"

#include "NodeGraphicsItem.h"
#include "NodePortGraphicsItem.h"

#include "controllers/NodeGraphController.h"
#include "model/Node.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>

NodeGraphScene::NodeGraphScene(QObject* parent, NodeGraphController& controller)
: QGraphicsScene(parent)
, m_controller(controller)
{
}

void NodeGraphScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
	if (dynamic_cast<NodePortGraphicsItem*>(item) != nullptr)
	{
		NodePortGraphicsItem* port = dynamic_cast<NodePortGraphicsItem*>(item);
		m_controller.set_first_connection_port(port->node_port());
		port->select();
		const QPointF& mouse_pos = port->scenePos() + QPointF(10, 10);
		m_line_start_pos = mouse_pos;
		m_line_edit_item = new QGraphicsLineItem(mouse_pos.x(), mouse_pos.y(), mouse_pos.x(), mouse_pos.y());
		addItem(m_line_edit_item);
	}
	else if (item == nullptr)
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

void NodeGraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (m_line_edit_item != nullptr)
	{
		const QPointF& mouse_pos = event->scenePos();
		m_line_edit_item->setLine(QLineF(m_line_start_pos, mouse_pos));
	}
	QGraphicsScene::mouseMoveEvent(event);
}

void NodeGraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	QList<QGraphicsItem*> itemlist = items(event->scenePos());
	bool found = false;
	for (QGraphicsItem* item : itemlist)
	{
		NodePortGraphicsItem* port_gfx_item = dynamic_cast<NodePortGraphicsItem*>(item);
		if (port_gfx_item != nullptr)
		{
			m_controller.set_second_connection_port(port_gfx_item->node_port());
			if (m_controller.create_connection())
			{
				m_line_edit_item->setLine(QLineF(m_line_start_pos, item->scenePos() + QPointF(10, 10)));
				found = true;
			}
		}
	}

	if (!found)
		removeItem(m_line_edit_item);


	m_line_edit_item = nullptr;
	QGraphicsScene::mouseReleaseEvent(event);
}
