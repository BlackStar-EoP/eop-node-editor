#pragma once

#include <controllers/NodeGraphController.h>
#include <model/NodeGraph.h>

#include <QWidget>
#include <QVector>

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

signals:
	void node_graph_changed();
	void message(const QString& message);

private:
	QGraphicsView* m_view = nullptr;
	NodeGraphScene* m_scene = nullptr;
	NodeGraphController m_controller;
	NodeGraph m_node_graph;
	NodeFactory* m_node_factory = nullptr;
};