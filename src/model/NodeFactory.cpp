#include "NodeFactory.h"

#include <assert.h>


NodeType::NodeType(const QString& display_name, const QString& node_type, ENodePolicy node_policy)
: m_display_name(display_name)
, m_node_type(node_type)
, m_node_policy(node_policy)
{
}

const QString& NodeType::display_name() const
{
	return m_display_name;
}

const QString& NodeType::node_type() const
{
	return m_node_type;
}

NodeType::ENodePolicy NodeType::node_policy() const
{
	return m_node_policy;
}

bool NodeType::operator==(const NodeType& node_type) const
{
	return node_type.m_node_type == m_node_type;
}

const QSet<NodeType>& NodeFactory::node_types() const
{
	return m_node_types;
}

const NodeType& NodeFactory::current_node_type() const
{
	return m_current_node_type;
}

void NodeFactory::set_current_node_type(const NodeType& node_type)
{
	m_current_node_type = node_type;
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