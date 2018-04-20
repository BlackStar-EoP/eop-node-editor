#pragma once

#include <QString>
#include <QtCore/qhash.h>

class NodeType
{
public:
	enum ENodePolicy
	{
		POLICY_MIN_ZERO_MAX_ONE,
		POLICY_MIN_ZERO_MAX_INF,
		POLICY_MIN_ONE_MAX_ONE,
		POLICY_MIN_ONE_MAX_INF
	};

public:
	NodeType(const QString& display_name, const QString& node_type, ENodePolicy node_policy);
	NodeType() = default;

	const QString& display_name() const;
	const QString& node_type() const;
	ENodePolicy node_policy() const;

public:
	bool operator == (const NodeType& node_type) const;

private:
	QString m_display_name;
	QString m_node_type;
	ENodePolicy m_node_policy = POLICY_MIN_ZERO_MAX_INF;
};

inline uint qHash(const NodeType& node_type, uint seed)
{
	return qHash(node_type.node_type(), seed);
}
