#include "NodeGraphScene.h"

#include "NodeConnectionGraphicsItem.h"
#include "NodeGraphicsItem.h"
#include "NodePortGraphicsItem.h"
#include "EditorColorScheme.h"
#include "controllers/NodeGraphController.h"

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
	QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
	if (dynamic_cast<NodePortGraphicsItem*>(item) != nullptr)
	{
		NodePortGraphicsItem* port = dynamic_cast<NodePortGraphicsItem*>(item);
		m_controller.set_first_connection_port(&port->node_port_model());
		port->select();

		m_line_edit_item = new NodeConnectionGraphicsItem();
		m_line_edit_item->set_first_port(port);
		addItem(m_line_edit_item);
	}
	else if (item == nullptr)
	{
		NodeModel* model = m_controller.add_node(event->scenePos());
		if (model != nullptr)
		{
			NodeGraphicsItem* nodeitem = new NodeGraphicsItem(model);
			model->register_node_model_listener(nodeitem);
			addItem(nodeitem);
		}
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
			m_controller.set_second_connection_port(&port_gfx_item->node_port_model());
			NodeConnection* connection = m_controller.create_connection();
			if (connection != nullptr)
			{
				m_line_edit_item->set_second_port(port_gfx_item);
				m_line_edit_item->set_connection(connection);
				found = true;
			}
		}
	}

	if (!found)
		removeItem(m_line_edit_item);


	m_line_edit_item = nullptr;
	QGraphicsScene::mouseReleaseEvent(event);
}

void NodeGraphScene::keyPressEvent(QKeyEvent* keyEvent)
{
	if (keyEvent->key() == Qt::Key_Delete)
	{
		QList<QGraphicsItem*> items = selectedItems();
		for (QGraphicsItem* item : items)
		{
			if (dynamic_cast<NodeGraphicsItem*>(item) != nullptr)
			{
				NodeGraphicsItem* nodeGraphicsItem = dynamic_cast<NodeGraphicsItem*>(item);
				m_controller.delete_node(nodeGraphicsItem->node_model());
			}
			//else if (dynamic_cast<NodePortGraphicsItem*>(item) != nullptr)
			//{
			//	NodePortGraphicsItem* nodePortGraphicsItem = dynamic_cast<NodePortGraphicsItem*>(item);
			//}
			else if (dynamic_cast<NodeConnectionGraphicsItem*>(item) != nullptr)
			{
				NodeConnectionGraphicsItem* nodeConnectionGraphicsItem = dynamic_cast<NodeConnectionGraphicsItem*>(item);
				m_controller.delete_connection(nodeConnectionGraphicsItem->connection());
			}
		}
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