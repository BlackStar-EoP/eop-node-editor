#include "NodePort.h"

NodePort::NodePort(NodePortModel* model, const Node* const node)
: m_model(model)
, m_node(node)
{

}

NodePortModel::EPortType NodePort::port_type() const
{
	return m_model->port_type();
}

QString NodePort::port_label() const
{
	return m_model->port_label();
}

const Node* const NodePort::node() const
{
	return m_node;
}

const NodeConnection* const NodePort::connection() const
{
	return m_connection;
}

void NodePort::set_connection(const NodeConnection* const connection)
{
	m_connection = connection;
}

void NodePort::remove_connection()
{
	m_connection = nullptr;
}
