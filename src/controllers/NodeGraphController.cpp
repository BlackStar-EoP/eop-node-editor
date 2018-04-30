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

NodeModel* NodeGraphController::add_node(const QPointF& position)
{
	assert(m_node_factory != nullptr);
	NodeModel* model = m_node_factory->create_node_model_and_set_type();
	if (model == nullptr)
	{
		emit message("No node type selected!");
		return nullptr;
	}

	if (!m_node_graph.is_add_allowed(model))
	{
		delete model;
		return nullptr;
	}

	model->set_position(position);
	model->create_port_models();
	m_node_graph.give_node(model);
	notify_node_graph_changed();
	return model;
}

void NodeGraphController::delete_node(NodeModel* node_model)
{
	m_node_graph.remove_node(node_model);
}

void NodeGraphController::delete_connection(NodeConnection* connection)
{
	delete connection;
}

void NodeGraphController::set_first_connection_port(NodePortModel* port)
{
	m_first_connection_port = port;
}

void NodeGraphController::set_second_connection_port(NodePortModel* port)
{
	m_second_connection_port = port;
}

NodeConnection* NodeGraphController::create_connection()
{
	if (m_first_connection_port == nullptr || m_second_connection_port == nullptr)
	{
		emit message("Connection port is nullptr, this is VERY bad!");
		return nullptr;
	}

	if ((m_first_connection_port->num_connections() > 0) && (!m_first_connection_port->supports_multiple_connections()))
	{
		assert(m_first_connection_port->num_connections() == 1);
		emit message("First connection port already has a connection and does not support multiple!");
		return nullptr;
	}

	if ((m_second_connection_port->num_connections() > 0) && (!m_second_connection_port->supports_multiple_connections()))
	{
		assert(m_second_connection_port->num_connections() == 1);
		emit message("Second connection port already has a connection and does not support multiple!");
		return nullptr;
	}

	if (m_first_connection_port == m_second_connection_port)
	{
		emit message("Connection ports are the same!");
		return nullptr;
	}

	if (m_first_connection_port->port_type() == m_second_connection_port->port_type())
	{
		emit message("Port types are the same!");
		return nullptr;
	}

	if (m_first_connection_port->node_model() == m_second_connection_port->node_model())
	{
		emit message("Connection to the same node is not allowed!");
		return nullptr;
	}

	if (m_first_connection_port->has_connection(m_second_connection_port))
	{
		emit message("Connection already exists!");
		return nullptr;
	}

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
	{
		emit message("Connection between these port types is not allowed!");
		return nullptr;
	}

	// If output goes to an input of node earlier in graph, we have a circular dependency
	if (m_node_graph.scan_left(output_port->node_model(), input_port->node_model()))
	{
		emit message("Circular dependency found, this is not allowed!");
		return nullptr;
	}

	m_first_connection_port = nullptr;
	m_second_connection_port = nullptr;

	NodeConnection* connection = new NodeConnection(input_port, output_port);
	emit message("Connection created!");
	notify_node_graph_changed();
	return connection;
}

void NodeGraphController::notify_node_graph_changed()
{
	emit node_graph_changed();
}