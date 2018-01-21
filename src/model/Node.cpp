#include "Node.h"

Node::Node(const QString& name, const QPointF& position)
: m_position(position)
, m_name(name)
{

}

Node::~Node()
{

}

void Node::add_input(const QString& port_label)
{
	m_input_ports.push_back(new NodePort(NodePort::INPUT, port_label, this));
}

void Node::add_output(const QString& port_label)
{
	m_output_ports.push_back(new NodePort(NodePort::OUTPUT, port_label, this));
}

void Node::set_position(const QPointF& position)
{
	m_position = position;
}

const QString& Node::name() const
{
	return m_name;
}

const QPointF& Node::position() const
{
	return m_position;
}

const QVector<NodePort*>& Node::input_ports() const
{
	return m_input_ports;
}

const QVector<NodePort*>& Node::output_ports() const
{
	return m_output_ports;
}

uint32_t Node::num_ports() const
{
	return m_input_ports.size() + m_output_ports.size();
}