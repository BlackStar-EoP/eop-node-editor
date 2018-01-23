#include "Node.h"

#include "NodeModel.h"

Node::Node(const QPointF& position, NodeModel* model)
: m_position(position)
, m_model(model)
{

}

Node::~Node()
{
	delete m_model;
}

void Node::set_position(const QPointF& position)
{
	m_position = position;
}

const QString Node::title() const
{
	return m_model->title();
}

NodeModel* Node::model() const
{
	return m_model;
}

const QPointF& Node::position() const
{
	return m_position;
}

bool Node::is_orphan() const
{
	//for (NodePort* input : m_input_ports)
	//{
	//	if (input->connection() != nullptr)
	//		return false;
	//}

	//for (NodePort* output : m_output_ports)
	//{
	//	if (output->connection() != nullptr)
	//		return false;
	//}

	return true;
}