#include "NodeConnection.h"

#include "model/NodePortModel.h"

NodeConnection::NodeConnection(NodePortModel* input, NodePortModel* output)
: m_input(input)
, m_output(output)
{
	m_input->add_connection(this);
	m_output->add_connection(this);
}

NodeConnection::~NodeConnection()
{
	m_input->remove_connection(this);
	m_output->remove_connection(this);
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
