#include "NodeGraph.h"

#include "model/NodeModel.h"
#include "model/NodeConnection.h"
#include "model/NodePortModel.h"
#include "model/NodeFactory.h"

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

void NodeGraph::remove_node(NodeModel* node)
{
	int index = m_nodes.indexOf(node);
	if (index != -1)
	{
		m_nodes.remove(index);
		delete node;
	}
}

void NodeGraph::clear()
{
	for (NodeModel* model : m_nodes)
	{
		delete model;
	}

	m_nodes.clear();
}

bool NodeGraph::scan_left(NodeModel* start, NodeModel* target) const
{
	if (start == target)
		return true;

	for (uint32_t i = 0; i < start->num_input_ports(); ++i)
	{
		const NodePortModel* port_model = start->input_port_model(i);
		uint32_t num_connections = port_model->num_connections();
		if (num_connections == 0)
			continue;
		
		for (uint32_t connection = 0; connection < num_connections; ++connection)
		{
			NodeModel* left_node = port_model->connection(connection)->output()->node_model();
			if (scan_left(left_node, target))
				return true;
		}
	}

	return false;
}

bool NodeGraph::scan_right(NodeModel* start, NodeModel* target) const
{
	Q_UNUSED(start);
	Q_UNUSED(target);
	return false;
}

bool NodeGraph::is_add_allowed(NodeModel* model) const
{
	if (model->node_type().node_policy() == NodeType::POLICY_MIN_ONE_MAX_ONE)
	{
		uint32_t num_policy_one_max_one_nodes = 0;
		for (NodeModel* m : m_nodes)
		{
			if (m->node_type() == model->node_type())
				++num_policy_one_max_one_nodes;
		}
		assert(num_policy_one_max_one_nodes <= 1);
		return num_policy_one_max_one_nodes < 1;
	}

	// TODO add the rest
	return true;
}

const QVector<NodeModel*>& NodeGraph::nodes() const
{
	return m_nodes;
}