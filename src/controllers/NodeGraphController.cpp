#include "NodeGraphController.h"

#include "model/NodeModel.h"
#include "model/NodeGraph.h"
#include "model/NodeFactory.h"
#include "model/NodePortModel.h"

#include <QPoint>
#include <assert.h>

NodeGraphController::NodeGraphController(NodeGraph& node_graph)
: m_node_graph(node_graph)
{
}

void NodeGraphController::set_node_factory(NodeFactory* factory)
{
	assert(m_node_factory == nullptr);
	m_node_factory = factory;
}

void NodeGraphController::set_first_connection_port(NodePortModel* port)
{
	m_first_connection_port = port;
}

void NodeGraphController::set_second_connection_port(NodePortModel* port)
{
	m_second_connection_port = port;
}

const NodeConnection* NodeGraphController::create_connection()
{
	if (m_first_connection_port == nullptr || m_second_connection_port == nullptr)
		return nullptr;

	// TODO, input ports can only have 1 connection, but output ports can have multiple
	// TODO, make connections vector, put method has_connections() return size > 0 in
	// TODO check for connections here, put method multiple_connections_allowed() in
	if (m_first_connection_port->connection() != nullptr || m_second_connection_port->connection() != nullptr)
		return nullptr;

	if (m_first_connection_port == m_second_connection_port)
		return nullptr;

	if (m_first_connection_port->port_type() == m_second_connection_port->port_type())
		return nullptr;

	if (m_first_connection_port->node_model() == m_second_connection_port->node_model())
		return nullptr;

	NodePortModel* input_port = nullptr;
	NodePortModel* output_port = nullptr;

	if (m_first_connection_port->port_type() == NodePortModel::INPUT)
	{
		input_port = m_first_connection_port;
		output_port = m_second_connection_port;
	}
	else if (m_second_connection_port->port_type() == NodePortModel::INPUT)
	{
		input_port = m_second_connection_port;
		output_port = m_first_connection_port;
	}

	if (!input_port->may_connect_to(*output_port) || !output_port->may_connect_to(*input_port))
		return nullptr;


	// If output goes to an input of node earlier in graph, we have a circular dependency
	if (m_node_graph.scan_left(output_port->node_model(), input_port->node_model()))
		return nullptr;


	// If input comes from node later in graph, we also have a circular dependency

	m_first_connection_port = nullptr;
	m_second_connection_port = nullptr;

	input_port->node_model()->on_connection(NodeConnection::INCOMING, output_port->node_model(), nullptr);
	output_port->node_model()->on_connection(NodeConnection::OUTGOING, input_port->node_model(), nullptr);

	NodeConnection* connection = new NodeConnection(*input_port, *output_port);
	m_node_graph.give_connection(connection);
	return connection;
}

NodeModel* NodeGraphController::add_node(const QPointF& position)
{
	assert(m_node_factory != nullptr);
	NodeModel* model = m_node_factory->create_node_model();
	if (model == nullptr)
	{
		emit message("No node type selected!");
		return nullptr;
	}

	model->set_position(position);
	model->create_port_models();
	m_node_graph.give_node(model);
	return model;
}