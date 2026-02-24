#include "NodeType.h"

NodeType::NodeType(const QString& display_name, const QString& node_type, ENodePolicy node_policy, const NodeCreator& node_creator)
: m_display_name(display_name)
, m_node_type(node_type)
, m_node_policy(node_policy)
, m_node_creator(node_creator)
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

NodeModel* NodeType::create() const
{
	return m_creator ? m_creator : nullptr;
}

bool NodeType::operator==(const NodeType& node_type) const
{
	return node_type.m_node_type == m_node_type;
}
