#include "NodeConnection.h"

#include "model/NodePortModel.h"

#include <assert.h>

NodeConnection::NodeConnection(NodePortModel* input, NodePortModel* output)
: m_input(input)
, m_output(output)
{
}

NodeConnection::~NodeConnection()
{
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

void NodeConnection::set_valid(bool valid)
{
	m_valid = valid;
}

bool NodeConnection::is_valid() const
{
	return m_valid;
}