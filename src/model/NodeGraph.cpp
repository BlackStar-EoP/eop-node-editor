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

bool NodeGraph::scan_left(NodeModel* start, NodeModel* target) const
{
	if (start == target)
		return true;

	for (uint32_t i = 0; i < start->num_input_ports(); ++i)
	{
		const NodePortModel* port_model = start->input_port_model(i);
		if (port_model->connection() == nullptr)
			continue;
		
		NodeModel* left_node = port_model->connection()->output().node_model();
		if (scan_left(left_node, target))
			return true;
	}

	return false;
}

bool NodeGraph::scan_right(NodeModel* start, NodeModel* target) const
{
	return false;
}
