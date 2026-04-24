#include "NodeGraphScene.h"

#include "NodeConnectionGraphicsItem.h"
#include "NodeGraphicsItem.h"
#include "EditorColorScheme.h"
#include "controllers/NodeGraphController.h"

//#include "model/NodeModel.h" TODO al geinclude in nodegraphicsitem vanwege interface
#include "model/NodePortModel.h"
#include "view/NodePortConnectorWidget.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QKeyEvent>
#include <QWidget>

#include <assert.h>
#include <qobject.h>
#include <qsharedpointer.h>

NodeGraphScene::NodeGraphScene(QObject* parent, NodeGraphController& controller)
: QGraphicsScene(parent)
, m_controller(controller)
{
	connect(&controller, SIGNAL(node_added(NodeModel*)), this, SLOT(node_added(NodeModel*)));
	connect(&controller, SIGNAL(connection_created(NodeConnection*)), this, SLOT(connection_created(NodeConnection*)));
	connect(&EditorColorScheme::instance(), &EditorColorScheme::colorsChanged, this, [this]() {
		invalidate();
	});
}

void NodeGraphScene::build_scene_from_graph(const QVector<NodeModel*>& nodes)
{
    for (NodeModel* node_model : nodes)
    {
        NodeGraphicsItem* node_gfx_item = new NodeGraphicsItem(node_model);
        node_model->register_node_model_listener(node_gfx_item);
        addItem(node_gfx_item);
    }

    for (NodeModel* node_model : nodes)
    {
        for (NodePortModel* port : node_model->input_ports())
        {
            for (NodeConnection* connection : port->connections())
            {
                NodeConnectionGraphicsItem* connection_gfx_item = new NodeConnectionGraphicsItem();
                connection_gfx_item->add_port(connection->input()->connector_widget());
                connection_gfx_item->add_port(connection->output()->connector_widget());
                connection_gfx_item->set_connection(connection);
                addItem(connection_gfx_item);
            }
        }
    }
}

void NodeGraphScene::clear_all()
{
    m_line_edit_item = nullptr;
    QGraphicsScene::clear();
}

NodePortConnectorWidget* NodeGraphScene::port_connector_at(const QPointF& scene_pos) const
{
    QList<QGraphicsItem*> item_list = items(scene_pos);

    if (item_list.size() > 0)
    {
        for (QGraphicsItem* item : item_list)
        {
            if (item->isWidget())
            {
                QGraphicsProxyWidget* proxy = qgraphicsitem_cast<QGraphicsProxyWidget*>(item);
                if (proxy != nullptr) {
                    QWidget* widget = proxy->widget();
                    QPointF proxyLocalPos = proxy->mapFromScene(scene_pos);
                    QPoint widgetLocalPos = widget->mapFrom(proxy->widget()->parentWidget(), proxyLocalPos.toPoint());
                    QWidget* childWidget = widget->childAt(widgetLocalPos);

                    return qobject_cast<NodePortConnectorWidget*>(childWidget);
                }
            }
        }
    }
    return nullptr;
}

void NodeGraphScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton)
	{
        if (!items(event->scenePos()).empty())
        {
            if (NodePortConnectorWidget* port = port_connector_at(event->scenePos());
                    port != nullptr)
            {
                m_controller.set_first_connection_port(port->port_model());
                port->select();

                m_line_edit_item = new NodeConnectionGraphicsItem();
                m_line_edit_item->add_port(port);
                addItem(m_line_edit_item);
                return;
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
    if (m_line_edit_item != nullptr)
    {
        if (NodePortConnectorWidget* port = port_connector_at(event->scenePos());
                port != nullptr)
        {
            m_controller.set_second_connection_port(port->port_model());
            m_controller.create_connection();
        }

        delete m_line_edit_item;
        m_line_edit_item = nullptr;
        update();
    }

	QGraphicsScene::mouseReleaseEvent(event);
}

void NodeGraphScene::keyPressEvent(QKeyEvent* keyEvent)
{
	if (keyEvent->key() == Qt::Key_Delete || keyEvent->key() == Qt::Key_Backspace)
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
	painter->fillRect(rect, EditorColorScheme::gridMinorColor());
    const int gridSize = 25;
	painter->setPen(EditorColorScheme::contentBackgroundColor());

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

void NodeGraphScene::node_added(NodeModel* node_model)
{
	assert(node_model != nullptr);
    node_model->create_widget();
    node_model->sync_widget_from_model();
	NodeGraphicsItem* node_gfx_item = new NodeGraphicsItem(node_model);
	node_model->register_node_model_listener(node_gfx_item);
	addItem(node_gfx_item);
}

void NodeGraphScene::connection_created(NodeConnection* connection)
{
	NodeConnectionGraphicsItem* connection_gfx_item = new NodeConnectionGraphicsItem();

	connection_gfx_item->add_port(connection->input()->connector_widget());
	connection_gfx_item->add_port(connection->output()->connector_widget());
	connection_gfx_item->set_connection(connection);
	addItem(connection_gfx_item);
}