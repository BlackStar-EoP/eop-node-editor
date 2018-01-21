#include "NodeGraphController.h"

#include "model/Node.h"
#include "model/NodeGraph.h"

#include <QPoint>
#include <assert.h>

NodeGraphController::NodeGraphController(NodeGraph& node_graph)
: m_node_graph(node_graph)
{
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

	if (m_first_connection_port->connection() != nullptr || m_second_connection_port->connection() != nullptr)
		return nullptr;

	if (m_first_connection_port == m_second_connection_port)
		return nullptr;

	if (m_first_connection_port->port_type() == m_second_connection_port->port_type())
		return nullptr;

	if (m_first_connection_port->node() == m_second_connection_port->node())
		return nullptr;

	NodePort* input_port = nullptr;
	NodePort* output_port = nullptr;

	if (m_first_connection_port->port_type() == NodePort::INPUT)
	{
		input_port = m_first_connection_port;
		output_port = m_second_connection_port;
	}
	else if (m_second_connection_port->port_type() == NodePort::INPUT)
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
	const char* names[] = { "Node1", "Node2", "Node3", "Node4", "Node5", "Node6", "Node7", };
	static int zooi = 0;
	Node* n = new Node(names[zooi++], position);
	m_node_graph.give_node(n);
	int inputs = rand() % 9 + 1;
	int outputs = rand() % 9 + 1;

	for (int i = 0; i < inputs; ++i)
		n->add_input("Input1");
	for (int i = 0; i < outputs; ++i)
		n->add_output("Output1");
	return n;
}