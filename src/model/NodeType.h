#pragma once

#include <QString>
#include <QtCore/qhash.h>

class NodeType
{
public:
	/** \brief Node policy that describes multiplicity of the node */
	enum ENodePolicy
	{
		POLICY_MIN_ZERO_MAX_ONE, /**< Minimum of zero, maximum of one */
		POLICY_MIN_ZERO_MAX_INF, /**< Minimum of zero, no maximum */
		POLICY_MIN_ONE_MAX_ONE, /**< Minimum of one, maximum of one */
		POLICY_MIN_ONE_MAX_INF /**< Minimum of one, no maximum */
	};

	/** \brief Enumeration to determine a node's purpose */
	enum ENodePurpose
	{
		NODE_PURPOSE_REGULAR, /**< Regular node */
		NODE_PURPOSE_SINK /**< Sink node, graph should have only one of these */
	};

public:
	NodeType(const QString& display_name, const QString& node_type, ENodePolicy node_policy, ENodePurpose purpose);
	NodeType() = default;

	const QString& display_name() const;
	const QString& node_type() const;
	ENodePolicy node_policy() const;
	ENodePurpose purpose() const;

public:
	bool operator == (const NodeType& node_type) const;

private:
	QString m_display_name = "";
	QString m_node_type = "";
	ENodePolicy m_node_policy = POLICY_MIN_ZERO_MAX_INF;
	ENodePurpose m_purpose = NODE_PURPOSE_REGULAR;
};

inline size_t qHash(const NodeType& node_type, uint seed)
{
	return qHash(node_type.node_type(), seed);
}
