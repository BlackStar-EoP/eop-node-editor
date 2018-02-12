#pragma once

#include <vector>

class QPointF;

class NodeGraph;
class NodeFactory;
class Node;
class NodePort;
class NodeConnection;

class NodeGraphController
{
public:
	NodeGraphController(NodeGraph& node_graph);
	~NodeGraphController() = default;

	void set_node_factory(NodeFactory* factory);
	Node* add_node(const QPointF& position);

	void set_first_connection_port(NodePort* port);
	void set_second_connection_port(NodePort* port);
	const NodeConnection* create_connection();

private:
	NodeGraph& m_node_graph;
	NodeFactory* m_node_factory = nullptr;
	NodePort* m_first_connection_port = nullptr;
	NodePort* m_second_connection_port = nullptr;
};