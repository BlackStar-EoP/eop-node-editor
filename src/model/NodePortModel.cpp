#include "NodePortModel.h"

#include "NodeConnection.h"

#include <assert.h>

NodePortModel::~NodePortModel()
{
	emit node_port_model_destroyed();
}

void NodePortModel::set_node_model(NodeModel* node_model)
{
	m_node_model = node_model;
}

NodeModel* NodePortModel::node_model() const
{
	return m_node_model;
}

NodeConnection* NodePortModel::connection(uint32_t index) const
{
	assert(static_cast<int32_t>(index) < m_connections.size());
	return m_connections[index];
}

void NodePortModel::add_connection(NodeConnection* connection)
{
	assert(m_connections.indexOf(connection) == -1);
	m_connections.push_back(connection);
}

void NodePortModel::remove_connection(NodeConnection* connection)
{
	int32_t index = m_connections.indexOf(connection);
	assert(index != -1);
	m_connections.remove(index);
}

uint32_t NodePortModel::num_connections() const
{
	return static_cast<uint32_t>(m_connections.size());
}

bool NodePortModel::has_connection(NodePortModel* other_port_model)
{
	for (NodeConnection* connection : m_connections)
	{
		if (connection->composed_by(this, other_port_model))
			return true;
	}

	return false;
}