#pragma once

#include <controllers/NodeGraphController.h>
#include <model/NodeGraph.h>

#include <QWidget>
#include <QVector>
#include <QJsonObject>

class QGraphicsView;
class NodeGraphScene;
class NodeFactory;
class NodeModel;

class NodeGraphWidget : public QWidget
{
	Q_OBJECT
public:
	NodeGraphWidget(QWidget* parent = nullptr);
	~NodeGraphWidget();

	void give_node_factory(NodeFactory* factory);

public:
	const QVector<NodeModel*>& nodes() const;
	void set_persisted();
	bool is_persisted() const;

	void new_graph();
	QJsonObject save_graph() const;
	void load_graph(const QJsonObject& json_data);

    /**
     * Replace the currently visible graph with the provided graph.
     * Drops the currently visible graph and takes ownership of the nodes in the graph.
     */
    void adopt_graph(NodeGraph& source_graph);

signals:
	void node_graph_changed();
	void message(const QString& message, bool status);

private:
    void clear();

private:
	QGraphicsView* m_view = nullptr;
	NodeGraphScene* m_scene = nullptr;
	NodeGraphController m_controller;
	NodeGraph m_node_graph;
	NodeFactory* m_node_factory = nullptr;
};