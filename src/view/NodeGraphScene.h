#pragma once

#include <QGraphicsScene>
#include <QVector>

class NodeGraphController;
class NodeConnectionGraphicsItem;
class NodeModel;
class NodeConnection;
class NodeGraphicsItem;

class NodeGraphScene : public QGraphicsScene
{
	Q_OBJECT
public:
	NodeGraphScene(QObject* parent, NodeGraphController& controller);

    void build_scene_from_graph(const QVector<NodeModel*>& nodes);

    /**
     * Remove all nodes and reset the state.
     */
    void clear_all();

private:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* keyEvent) override;
	void drawBackground(QPainter* painter, const QRectF& rect) override;
	NodeGraphicsItem* find_node_graphics_item(NodeModel* node_model);

private slots:
	void node_added(NodeModel* node_model);
	void connection_created(NodeConnection* connection);

private:
	NodeGraphController& m_controller;

	QVector<NodeGraphicsItem*> m_node_gfx_items;
    QVector<NodeConnectionGraphicsItem*> m_node_connection_gfx_items;
	NodeConnectionGraphicsItem* m_line_edit_item = nullptr;
	QPointF m_line_start_pos;

};