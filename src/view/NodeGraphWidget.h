#pragma once

#include <controllers/NodeGraphController.h>
#include <model/NodeGraph.h>

#include <QWidget>

class QGraphicsView;
class NodeGraphScene;
class NodeFactory;

class NodeGraphWidget : public QWidget
{
public:
	NodeGraphWidget(QWidget* parent = nullptr);
	~NodeGraphWidget();

	void give_node_factory(NodeFactory* factory);

private:
	QGraphicsView* m_view = nullptr;
	NodeGraphScene* m_scene = nullptr;
	NodeGraphController m_controller;
	NodeGraph m_node_graph;
	NodeFactory* m_node_factory = nullptr;
};