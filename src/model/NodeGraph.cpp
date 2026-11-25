#include "NodeGraph.h"

#include "controllers/NodeGraphController.h"
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
    clear();
}

void NodeGraph::set_controller(NodeGraphController* controller)
{
    m_controller = controller;
}

void NodeGraph::give_node(NodeModel* node)
{
    node->set_graph(this);
	m_nodes.push_back(node);
}

void NodeGraph::remove_node(NodeModel* node)
{
	int index = m_nodes.indexOf(node);
	if (index != -1)
	{
        for (NodePortModel* port : node->input_ports())
        {
            disconnect_all(port);
        }
        for (NodePortModel* port : node->output_ports())
        {
            disconnect_all(port);
        }
		m_nodes.remove(index);
		delete node;
	}
}

NodeConnectionCheck NodeGraph::check_connection(NodePortModel* input_port, NodePortModel* output_port) const
{
	if ((input_port->num_connections() > 0) && (not input_port->supports_multiple_connections()))
		return NodeConnectionCheck::INPUT_FULL;

	if ((output_port->num_connections() > 0) && (not output_port->supports_multiple_connections()))
		return NodeConnectionCheck::OUTPUT_FULL;

    if (input_port == output_port)
        return NodeConnectionCheck::SAME_PORT;

    if (input_port->port_type() == output_port->port_type())
        return NodeConnectionCheck::SAME_PORT_TYPE;

    if (input_port->owning_node() == output_port->owning_node())
        return NodeConnectionCheck::SAME_NODE;

    if (input_port->has_connection(output_port))
        return NodeConnectionCheck::ALREADY_CONNECTED;

    if (not input_port->may_connect_to(*output_port) || not output_port->may_connect_to(*input_port))
        return NodeConnectionCheck::TYPES_NOT_COMPATIBLE;

	// If output goes to an input of node earlier in graph, we have a circular dependency
    if (scan_left(output_port->owning_node(), input_port->owning_node()))
        return NodeConnectionCheck::CIRCULAR;

    return NodeConnectionCheck::OK;
}

NodeConnection* NodeGraph::connect(NodePortModel* input_port, NodePortModel* output_port)
{
    assert(input_port != nullptr);
    assert(output_port != nullptr);
    NodeConnection* connection = new NodeConnection(input_port, output_port);
    m_connections.append(connection);
    input_port->add_connection(connection);
    output_port->add_connection(connection);
    return connection;
}

void NodeGraph::disconnect(NodeConnection* connection)
{
    connection->input()->remove_connection(connection);
    connection->output()->remove_connection(connection);
    m_connections.removeAll(connection);
    delete connection;
}

void NodeGraph::move_endpoint(NodeConnection* connection, NodePortModel* from_port, NodePortModel* to_port)
{
    from_port->remove_connection_silent(connection);
    to_port->add_connection_silent(connection);
    connection->swap_endpoint(from_port, to_port);

    emit from_port->node_port_connections_changed();
    emit to_port->node_port_connections_changed();
}

void NodeGraph::disconnect_all(NodePortModel* port)
{
    QList<NodeConnection*> connections;
    for (NodeConnection* conn : m_connections)
    {
        if (conn->input() == port || conn->output() == port)
        {
            connections.append(conn);
        }
    }
    for (NodeConnection* conn : connections)
    {
        disconnect(conn);
    }
}

void NodeGraph::clear()
{
    while (!m_connections.isEmpty())
	{
        disconnect(m_connections.first());
	}

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
			NodeModel* left_node = port_model->connection(connection)->output()->owning_node();
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

void NodeGraph::notify_node_graph_changed()
{
    if (m_controller != nullptr)
        m_controller->notify_node_graph_changed();
}

const QVector<NodeModel*>& NodeGraph::nodes() const
{
	return m_nodes;
}

const QVector<NodeConnection*>& NodeGraph::connections() const
{
    return m_connections;
}