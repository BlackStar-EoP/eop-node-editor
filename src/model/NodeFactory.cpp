#include "NodeFactory.h"

#include <assert.h>


const QString& NodeFactory::current_node_type() const
{
	return m_current_node_type;
}

void NodeFactory::register_node_type(const QString& node_type)
{
	assert(!m_node_types.contains(node_type));
	m_node_types.insert(node_type);
}

void NodeFactory::select_node_type(const QString& node_type)
{
	assert(m_node_types.contains(node_type));
	m_current_node_type = node_type;
}