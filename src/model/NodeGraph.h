#pragma once

#include "Node.h"
#include "NodeConnection.h"
#include <vector>

class Node;
class NodeConnection;

class NodeGraph
{
public:
	NodeGraph();
	~NodeGraph();

	void give_node(Node* node);
	void give_connection(NodeConnection* connection);

	// Circular dependency checks
	bool scan_left(const Node* start, const Node* target) const;
	bool scan_right(const Node* start, const Node* target) const;

private:
	std::vector<Node*> m_nodes;
	std::vector<NodeConnection*> m_connections;
};