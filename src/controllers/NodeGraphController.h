#pragma once

#include <vector>

class QPointF;

class NodeGraph;
class NodeFactory;
class NodeConnection;
class NodeModel;
class NodePortModel;

class NodeGraphController
{
public:
	NodeGraphController(NodeGraph& node_graph);
	~NodeGraphController() = default;

	void set_node_factory(NodeFactory* factory);
	NodeModel* add_node(const QPointF& position);

	void set_first_connection_port(NodePortModel* port);
	void set_second_connection_port(NodePortModel* port);
	const NodeConnection* create_connection();

private:
	NodeGraph& m_node_graph;
	NodeFactory* m_node_factory = nullptr;
	NodePortModel* m_first_connection_port = nullptr;
	NodePortModel* m_second_connection_port = nullptr;
};