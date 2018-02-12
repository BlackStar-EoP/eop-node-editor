#include "NodeGraphController.h"

#include "model/Node.h"
#include "model/NodeModel.h"
#include "model/NodeGraph.h"
#include "model/NodeFactory.h"

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

void NodeGraphController::set_first_connection_port(NodePort* port)
{
	m_first_connection_port = port;
}

void NodeGraphController::set_second_connection_port(NodePort* port)
{
	m_second_connection_port = port;
}

const NodeConnection* NodeGraphController::create_connection()
{
	if (m_first_connection_port == nullptr || m_second_connection_port == nullptr)
		return nullptr;

	if (m_first_connection_port == m_second_connection_port)
		return nullptr;

	if (m_first_connection_port->port_type() == m_second_connection_port->port_type())
		return nullptr;

	if (m_first_connection_port->node() == m_second_connection_port->node())
		return nullptr;

	NodePort* input_port = nullptr;
	NodePort* output_port = nullptr;

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

	// If output goes to an input of node earlier in graph, we have a circular dependency
	if (m_node_graph.scan_left(output_port->node(), input_port->node()))
		return nullptr;

	// If input comes from node later in graph, we also have a circular dependency

	m_first_connection_port = nullptr;
	m_second_connection_port = nullptr;

	NodeConnection* connection = new NodeConnection(*input_port, *output_port);
	m_node_graph.give_connection(connection);
	return connection;
}

Node* NodeGraphController::add_node(const QPointF& position)
{
	assert(m_node_factory != nullptr);
	NodeModel* model = m_node_factory->create_node_model();
	if (model == nullptr)
	{
		assert(false);
		return nullptr;
	}

	model->create_port_models();
	Node* n = new Node(position, model);
	m_node_graph.give_node(n);
	return n;
}