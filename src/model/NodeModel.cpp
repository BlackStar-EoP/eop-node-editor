#include "NodeModel.h"

NodeModel::~NodeModel()
{
	emit node_model_destroyed();
}

void NodeModel::node_model_changed()
{
	for (INodeModelListener* l : m_node_model_listeners)
	{
		l->node_model_changed();
	}
}

void NodeModel::register_node_model_listener(INodeModelListener* listener)
{
	m_node_model_listeners.push_back(listener);
}

void NodeModel::set_position(const QPointF& position)
{
	m_position = position;
}

const QPointF& NodeModel::position() const
{
	return m_position;
}

bool NodeModel::is_orphan() const
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