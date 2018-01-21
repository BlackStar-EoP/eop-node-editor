#include "NodeGraphController.h"

#include "model/Node.h"
#include "model/NodeGraph.h"

#include <QPoint>
#include <assert.h>

NodeGraphController::NodeGraphController(NodeGraph& node_graph)
: m_node_graph(node_graph)
{
}

void NodeGraphController::set_first_connection_port(const NodePort& port)
{
	m_first_connection_port = &port;
}

void NodeGraphController::set_second_connection_port(const NodePort& port)
{
	m_second_connection_port = &port;
}

bool NodeGraphController::create_connection()
{
	if (m_first_connection_port == m_second_connection_port)
		return false;

	if (m_first_connection_port->port_type() == m_second_connection_port->port_type())
		return false;

	NodeConnection* connection = nullptr;
	if (m_first_connection_port->port_type() == NodePort::INPUT)
		connection = new NodeConnection(*m_first_connection_port, *m_second_connection_port);
	else if (m_second_connection_port->port_type() == NodePort::INPUT)
		connection = new NodeConnection(*m_second_connection_port, *m_first_connection_port);
	
	assert(connection);
	m_node_graph.give_connection(connection);
}

Node* NodeGraphController::add_node(const QPointF& position)
{
	Node* n = new Node(position);
	m_node_graph.give_node(n);
	int inputs = rand() % 10;
	int outputs = rand() % 10;

	for (int i = 0; i < inputs; ++i)
		n->add_input("Input1");
	for (int i = 0; i < outputs; ++i)
		n->add_output("Output1");
	return n;
}