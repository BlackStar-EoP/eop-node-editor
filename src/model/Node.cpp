#include "Node.h"

#include "NodeModel.h"

Node::Node(const QString& name, const QPointF& position, NodeModel* model)
: m_position(position)
, m_name(name)
, m_model(model)
{

}

Node::~Node()
{
	delete m_model;
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

bool Node::is_orphan() const
{
	for (NodePort* input : m_input_ports)
	{
		if (input->connection() != nullptr)
			return false;
	}

	for (NodePort* output : m_output_ports)
	{
		if (output->connection() != nullptr)
			return false;
	}

	return true;
}