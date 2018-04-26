#pragma once

#include <QVector>

class NodeConnection;
class NodeModel;
class NodePolicy;

class NodeGraph
{
public:
	NodeGraph();
	~NodeGraph();

	void give_node(NodeModel* node);
	void remove_node(NodeModel* node);
	void give_connection(NodeConnection* connection);
	void remove_connection(NodeConnection* connection);

	// Circular dependency checks
	bool scan_left(NodeModel* start, NodeModel* target) const;
	bool scan_right(NodeModel* start, NodeModel* target) const;

	bool is_add_allowed(NodeModel* model) const;

public:
	const QVector<NodeModel*>& nodes() const;

private:
	QVector<NodeModel*> m_nodes;
	QVector<NodeConnection*> m_connections;
};