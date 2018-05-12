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

#include <assert.h>

NodeGraphScene::NodeGraphScene(QObject* parent, NodeGraphController& controller)
: QGraphicsScene(parent)
, m_controller(controller)
{
	connect(&controller, SIGNAL(node_added(NodeModel*)), this, SLOT(node_added(NodeModel*)));
	connect(&controller, SIGNAL(connection_created(NodeConnection*)), this, SLOT(connection_created(NodeConnection*)));
}

void NodeGraphScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton)
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
			m_controller.add_node(event->scenePos());
		}
	}
	else if (event->buttons() == Qt::RightButton)
	{
	}
	QGraphicsScene::mousePressEvent(event);
}

void NodeGraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton && m_line_edit_item != nullptr)
	{
		const QPointF& mouse_pos = event->scenePos();
		m_line_edit_item->update_line(mouse_pos);
	}
	else if (event->buttons() == Qt::RightButton)
	{

	}
	else
	{
		QGraphicsScene::mouseMoveEvent(event);
	}
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
				delete m_line_edit_item;
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
		delete m_line_edit_item;
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
			m_node_gfx_items.remove(m_node_gfx_items.indexOf(nodeGraphicsItem));
		}

		for (NodeConnectionGraphicsItem* nodeConnectionGraphicsItem : connectionItemsToBeDeleted)
		{
			m_controller.delete_connection(nodeConnectionGraphicsItem->connection());
		}

		update();
		m_controller.notify_node_graph_changed();
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

NodeGraphicsItem* NodeGraphScene::find_node_graphics_item(NodeModel* node_model)
{
	for (NodeGraphicsItem* node_gfx_item : m_node_gfx_items)
	{
		if (node_gfx_item->node_model() == node_model)
			return node_gfx_item;
	}

	assert(false);
	return nullptr;
}

void NodeGraphScene::node_added(NodeModel* node_model)
{
	assert(node_model != nullptr);
	NodeGraphicsItem* node_gfx_item = new NodeGraphicsItem(node_model);
	node_model->register_node_model_listener(node_gfx_item);
	m_node_gfx_items.push_back(node_gfx_item);
	addItem(node_gfx_item);
}

void NodeGraphScene::connection_created(NodeConnection* connection)
{
	NodeGraphicsItem* input_node = find_node_graphics_item(connection->input()->node_model());
	NodeGraphicsItem* output_node = find_node_graphics_item(connection->output()->node_model());
	NodeConnectionGraphicsItem* connection_gfx_item = new NodeConnectionGraphicsItem();

	connection_gfx_item->set_first_port(input_node->find_input_port(connection->input()));
	connection_gfx_item->set_second_port(output_node->find_output_port(connection->output()));
	connection_gfx_item->set_connection(connection);
	addItem(connection_gfx_item);
}