#include "NodeConnection.h"

#include "model/NodePort.h"

NodeConnection::NodeConnection(NodePort& input, NodePort& output)
: m_input(input)
, m_output(output)
{
	m_input.set_connection(this);
	m_output.set_connection(this);
}

NodeConnection::~NodeConnection()
{
	m_input.remove_connection();
	m_output.remove_connection();
}

const NodePort& NodeConnection::input() const
{
	return m_input;
}

const NodePort& NodeConnection::output() const
{
	return m_output;
}
