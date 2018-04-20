#pragma once

#include <stdint.h>
#include <QString>
#include <QSet>
#include "NodeType.h"

class NodeModel;

class NodeFactory
{
public:
	NodeFactory() = default;
	virtual ~NodeFactory() = default;

	virtual NodeModel* create_node_model() = 0;
	NodeModel* create_node_model_and_set_type();

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