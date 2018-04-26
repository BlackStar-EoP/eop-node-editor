#include "NodeFactory.h"

#include "NodeModel.h"
#include <assert.h>

NodeModel* NodeFactory::create_node_model_and_set_type()
{
	NodeModel* model = create_node_model();
	if (model != nullptr)
		model->set_node_type(current_node_type());
	return model;
}

const QSet<NodeType>& NodeFactory::node_types() const
{
	return m_node_types;
}

const NodeType& NodeFactory::current_node_type() const
{
	return m_current_node_type;
}

void NodeFactory::set_current_node_type(const QString& node_type)
{
	for (const NodeType& type : m_node_types)
	{
		if (type.node_type() == node_type)
		{
			m_current_node_type = type;
			return;
		}
	}

	assert(false);
}

void NodeFactory::register_node_type(const NodeType& node_type)
{
	assert(!m_node_types.contains(node_type));
	m_node_types.insert(node_type);
}

void NodeFactory::select_node_type(const NodeType& node_type)
{
	assert(m_node_types.contains(node_type));
	m_current_node_type = node_type;
}