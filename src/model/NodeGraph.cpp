#include "NodeGraph.h"

#include "model/NodeModel.h"
#include "model/NodeConnection.h"
#include "model/NodePortModel.h"

#include <assert.h>

NodeGraph::NodeGraph()
{
}

NodeGraph::~NodeGraph()
{
}

void NodeGraph::give_node(NodeModel* node)
{
	m_nodes.push_back(node);
}

void NodeGraph::give_connection(NodeConnection* connection)
{
	assert(connection);
	m_connections.push_back(connection);
}

bool NodeGraph::scan_left(const NodeModel* start, const NodeModel* target) const
{
	// TODO fix
	//const QVector<NodePort*>& input_ports = start->input_ports();
	//
	//if (start == target)
	//	return true;

	//for (const NodePort* port : input_ports)
	//{
	//	if (port->connection() == nullptr)
	//		continue;
	//	
	//	const Node* left_node = port->connection()->output().node();
	//	if (scan_left(left_node, target))
	//		return true;
	//}

	return false;
}

bool NodeGraph::scan_right(const NodeModel* start, const NodeModel* target) const
{
	return false;
}
