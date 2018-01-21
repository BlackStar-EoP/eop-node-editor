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

private:
	std::vector<Node*> m_nodes;
	std::vector<NodeConnection*> m_connections;
};