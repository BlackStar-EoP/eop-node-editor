#include "NodeGraph.h"

#include "model/Node.h"
#include "model/NodeConnection.h"
#include "model/NodePort.h"

#include <assert.h>

NodeGraph::NodeGraph()
{
}

NodeGraph::~NodeGraph()
{
}

void NodeGraph::give_node(Node* node)
{
	m_nodes.push_back(node);
}

void NodeGraph::give_connection(NodeConnection* connection)
{
	assert(connection);
	m_connections.push_back(connection);
}

bool NodeGraph::scan_left(const Node* start, const Node* target) const
{
	const QVector<NodePort*>& input_ports = start->input_ports();
	
	if (start == target)
		return true;

	for (const NodePort* port : input_ports)
	{
		if (port->connection() == nullptr)
			continue;
		
		const Node* left_node = port->connection()->output().node();
		if (scan_left(left_node, target))
			return true;
	}

	return false;
}

bool NodeGraph::scan_right(const Node* start, const Node* target) const
{
	return false;
}
