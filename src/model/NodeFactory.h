#pragma once

#include <stdint.h>
#include <QString>
#include <QSet>

class NodeModel;

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

class NodeFactory
{
public:
	NodeFactory() = default;
	virtual ~NodeFactory() = default;

	virtual NodeModel* create_node_model() = 0;

	const QSet<NodeType>& node_types() const;
	const NodeType& current_node_type() const;
	void set_current_node_type(const NodeType& node_type);

protected:
	void register_node_type(const NodeType& nodeType);
	void select_node_type(const NodeType& node_type);

private:
	QSet<NodeType> m_node_types;
	NodeType m_current_node_type;
};