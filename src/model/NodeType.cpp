#include "NodeType.h"

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