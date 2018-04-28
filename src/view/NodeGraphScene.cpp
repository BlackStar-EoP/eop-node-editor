#include "NodeGraphScene.h"

#include "NodeConnectionGraphicsItem.h"
#include "NodeGraphicsItem.h"
#include "NodePortGraphicsItem.h"
#include "EditorColorScheme.h"
#include "controllers/NodeGraphController.h"

//#include "model/NodeModel.h" TODO al geinclude in nodegraphicsitem vanwege interface
#include "model/NodePortModel.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QKeyEvent>

NodeGraphScene::NodeGraphScene(QObject* parent, NodeGraphController& controller)
: QGraphicsScene(parent)
, m_controller(controller)
{
}

void NodeGraphScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QList<QGraphicsItem*> item_list = items(event->scenePos());

	if (item_list.size() > 0)
	{
		for (QGraphicsItem* item : item_list)
		{
			if (dynamic_cast<NodePortGraphicsItem*>(item) != nullptr)
			{
				NodePortGraphicsItem* port = dynamic_cast<NodePortGraphicsItem*>(item);
				m_controller.set_first_connection_port(port->port_model());
				port->select();

				m_line_edit_item = new NodeConnectionGraphicsItem();
				m_line_edit_item->set_first_port(port);
				addItem(m_line_edit_item);
				return;
			}
		}
	}
	else
	{
		NodeModel* model = m_controller.add_node(event->scenePos());
		if (model != nullptr)
		{
			NodeGraphicsItem* nodeitem = new NodeGraphicsItem(model);
			model->register_node_model_listener(nodeitem);
			addItem(nodeitem);
			return;
		}
	}

	QGraphicsScene::mousePressEvent(event);
}

void NodeGraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (m_line_edit_item != nullptr)
	{
		const QPointF& mouse_pos = event->scenePos();
		m_line_edit_item->update_line(mouse_pos);
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
			m_controller.set_second_connection_port(port_gfx_item->port_model());
			NodeConnection* connection = m_controller.create_connection();
			if (connection != nullptr)
			{
				m_line_edit_item->set_second_port(port_gfx_item);
				m_line_edit_item->set_connection(connection);
				found = true;
			}
			else
			{
				update();
			}
		}
	}

	if (!found)
	{
		removeItem(m_line_edit_item);
		update();
	}

	m_line_edit_item = nullptr;
	QGraphicsScene::mouseReleaseEvent(event);
}

void NodeGraphScene::keyPressEvent(QKeyEvent* keyEvent)
{
	if (keyEvent->key() == Qt::Key_Delete)
	{
		QList<QGraphicsItem*> items = selectedItems();
		QList<NodeGraphicsItem*> nodeItemsToBeDeleted;
		QList<NodeConnectionGraphicsItem*> connectionItemsToBeDeleted;

		for (QGraphicsItem* item : items)
		{
			if (dynamic_cast<NodeGraphicsItem*>(item) != nullptr)
			{
				NodeGraphicsItem* nodeGraphicsItem = dynamic_cast<NodeGraphicsItem*>(item);
				nodeItemsToBeDeleted.push_back(nodeGraphicsItem);
			}
			else if (dynamic_cast<NodeConnectionGraphicsItem*>(item) != nullptr)
			{
				NodeConnectionGraphicsItem* nodeConnectionGraphicsItem = dynamic_cast<NodeConnectionGraphicsItem*>(item);
				connectionItemsToBeDeleted.push_back(nodeConnectionGraphicsItem);
			}
		}

		for (NodeGraphicsItem* nodeGraphicsItem : nodeItemsToBeDeleted)
		{
			NodeModel* model = nodeGraphicsItem->node_model();
			QVector<NodeConnection*> connections;
			// Add all connections to one big list
			for (uint32_t port_nr = 0; port_nr < model->num_input_ports(); ++port_nr)
			{
				NodePortModel* port_model = model->input_port_model(port_nr);
				for (uint32_t con = 0; con < port_model->num_connections(); ++con)
				{
					connections.push_back(port_model->connection(con));
				}
			}

			for (uint32_t port_nr = 0; port_nr < model->num_output_ports(); ++port_nr)
			{
				NodePortModel* port_model = model->output_port_model(port_nr);
				for (uint32_t con = 0; con < port_model->num_connections(); ++con)
				{
					connections.push_back(port_model->connection(con));
				}
			}

			// Traverse all connection graphics items that will be deleted, in reversed order since we will remove them if they match
			// Otherwise they will be deleted when the node is deleted and then the loop that deletes the connections crashes
			for (int32_t con = connectionItemsToBeDeleted.size() - 1; con >= 0; --con)
			{
				NodeConnectionGraphicsItem* connectionGraphicsItem = connectionItemsToBeDeleted[con];
				if (connections.contains(connectionGraphicsItem->connection()))
				{
					connectionItemsToBeDeleted.removeAt(con);
				}
			}

			m_controller.delete_node(nodeGraphicsItem->node_model());
		}

		for (NodeConnectionGraphicsItem* nodeConnectionGraphicsItem : connectionItemsToBeDeleted)
		{
			m_controller.delete_connection(nodeConnectionGraphicsItem->connection());
		}

		update();
		return;
	}
	QGraphicsScene::keyPressEvent(keyEvent);
}

void NodeGraphScene::drawBackground(QPainter* painter, const QRectF& rect)
{
	painter->fillRect(rect, EditorColorScheme::gridMinorColor_);
    const int gridSize = 25;
	painter->setPen(EditorColorScheme::contentBackgroundColor_);
	
    qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += gridSize)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += gridSize)
        lines.append(QLineF(rect.left(), y, rect.right(), y));

//    qDebug() << lines.size();

    painter->drawLines(lines.data(), lines.size());
}