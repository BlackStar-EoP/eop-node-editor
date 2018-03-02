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