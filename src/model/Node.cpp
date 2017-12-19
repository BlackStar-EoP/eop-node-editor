#include "Node.h"

Node::Node(const QPointF& position)
: m_position(position)
{

}

Node::~Node()
{

}

void Node::add_input(const QString& port_label)
{
	m_input_ports.push_back(NodePort(NodePort::INPUT, port_label));
}

void Node::add_output(const QString& port_label)
{
	m_output_ports.push_back(NodePort(NodePort::OUTPUT, port_label));
}

const QPointF& Node::position() const
{
	return m_position;
}

const QVector<NodePort>& Node::input_ports() const
{
	return m_input_ports;
}

const QVector<NodePort>& Node::output_ports() const
{
	return m_output_ports;
}

uint32_t Node::num_ports() const
{
	return m_input_ports.size() + m_output_ports.size();
}