#pragma once

#include <vector>

class QPointF;

class NodeGraph;
class Node;
class NodePort;

class NodeGraphController
{
public:
	NodeGraphController(NodeGraph& node_graph);
	~NodeGraphController() = default;

	Node* add_node(const QPointF& position);

	void set_first_connection_port(const NodePort& port);
	void set_second_connection_port(const NodePort& port);
	bool create_connection();

private:
	NodeGraph& m_node_graph;
	const NodePort* m_first_connection_port = nullptr;
	const NodePort* m_second_connection_port = nullptr;
};