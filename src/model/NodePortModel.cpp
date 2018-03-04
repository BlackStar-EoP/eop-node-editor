#include "NodePortModel.h"

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

NodeConnection* NodePortModel::connection() const
{
	return m_connection;
}

void NodePortModel::set_connection(NodeConnection* connection)
{
	m_connection = connection;
}

void NodePortModel::remove_connection()
{
	m_connection = nullptr;
}