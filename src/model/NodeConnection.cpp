#include "NodeConnection.h"

#include "model/NodePortModel.h"

#include <assert.h>

NodeConnection::NodeConnection(NodePortModel* input, NodePortModel* output)
: m_input(input)
, m_output(output)
{
	m_input->add_connection(this);
	m_output->add_connection(this);
}

NodeConnection::~NodeConnection()
{
	if (m_output != nullptr)
		m_output->remove_connection(this);

	if (m_input != nullptr)
		m_input->remove_connection(this);

	emit connection_destroyed();
}

bool NodeConnection::composed_by(NodePortModel* first_port, NodePortModel* second_port) const
{
	return (m_input == first_port && m_output == second_port) ||
		   (m_input == second_port && m_output == first_port);
}


NodePortModel* NodeConnection::input() const
{
	return m_input;
}

NodePortModel* NodeConnection::output() const
{
	return m_output;
}

NodePortModel* NodeConnection::other(NodePortModel* port) const
{
	if (port == m_input)
		return m_output;
	else if (port == m_output)
		return m_input;

	assert(false);
	return nullptr;
}

void NodeConnection::remove_port(NodePortModel* port_model)
{
	if (m_input == port_model)
		m_input = nullptr;
	else if (m_output == port_model)
		m_output = nullptr;
	// TODO not too happy about this, but hopefully prevents crash
}