#pragma once

#include <stdint.h>
#include <QString>
#include <QSet>

class NodeModel;

class NodeFactory
{
public:
	NodeFactory() = default;
	virtual ~NodeFactory() = default;

	virtual NodeModel* create_node_model() = 0;
	const QString& current_node_type() const;

protected:
	void register_node_type(const QString& node_type);
	void select_node_type(const QString& node_type);

private:
	QSet<QString> m_node_types;
	QString m_current_node_type;
};