#pragma once

#include "NodeConnection.h"
#include <vector>

class NodeConnection;
class NodeModel;
class NodePolicy;

class NodeGraph
{
public:
	NodeGraph();
	~NodeGraph();

	void give_node(NodeModel* node);
	void give_connection(NodeConnection* connection);

	// Circular dependency checks
	bool scan_left(NodeModel* start, NodeModel* target) const;
	bool scan_right(NodeModel* start, NodeModel* target) const;

	bool is_add_allowed(NodeModel* model) const;

private:
	std::vector<NodeModel*> m_nodes;
	std::vector<NodeConnection*> m_connections;
};