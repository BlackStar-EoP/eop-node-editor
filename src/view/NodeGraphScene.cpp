#include "NodeGraphScene.h"

#include "NodeConnectionGraphicsItem.h"
#include "NodeGraphicsItem.h"
#include "NodePortGraphicsItem.h"

#include "controllers/NodeGraphController.h"

#include <QGraphicsSceneMouseEvent>

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
		NodeGraphicsItem* item = new NodeGraphicsItem(*model);
		model->register_node_model_listener(item);
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
			if (m_controller.create_connection() != nullptr)
			{
				m_line_edit_item->set_second_port(port_gfx_item);
				found = true;
			}
		}
	}

	if (!found)
		removeItem(m_line_edit_item);


	m_line_edit_item = nullptr;
	QGraphicsScene::mouseReleaseEvent(event);
}

void NodeGraphScene::drawBackground(QPainter* painter, const QRectF& rect)
{
    const int gridSize = 25;

    qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += gridSize)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += gridSize)
        lines.append(QLineF(rect.left(), y, rect.right(), y));

    qDebug() << lines.size();

    painter->drawLines(lines.data(), lines.size());
}