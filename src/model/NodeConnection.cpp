#include "NodeConnection.h"

#include "model/NodePortModel.h"

NodeConnection::NodeConnection(NodePortModel& input, NodePortModel& output)
: m_input(input)
, m_output(output)
{
	m_input.add_connection(this);
	m_output.add_connection(this);
}

NodeConnection::~NodeConnection()
{
	m_input.remove_connection(this);
	m_output.remove_connection(this);
	emit connection_destroyed();
}

const NodePortModel& NodeConnection::input() const
{
	return m_input;
}

const NodePortModel& NodeConnection::output() const
{
	return m_output;
}
