#pragma once

#include <QVector>

class NodeConnection;
class NodeGraphController;
class NodeModel;
class NodePolicy;
class NodePortModel;

class NodeGraph
{
public:
	NodeGraph();
	~NodeGraph();

    void set_controller(NodeGraphController* controller);

	void give_node(NodeModel* node);
	void remove_node(NodeModel* node);

    NodeConnection* connect(NodePortModel* input_port, NodePortModel* output_port);
    void disconnect(NodeConnection* connection);
    void disconnect_all(NodePortModel* port);

	void clear();

    /**
     * Release ownership of the nodes currently in the graph and return them.
     */
    QVector<NodeModel*> release_nodes();

	// Circular dependency checks
	bool scan_left(NodeModel* start, NodeModel* target) const;
	bool scan_right(NodeModel* start, NodeModel* target) const;

	bool is_add_allowed(NodeModel* model) const;

    /**
     * For internal use by NodeModel instances.
     */
    void notify_node_graph_changed();

public:
	const QVector<NodeModel*>& nodes() const;

private:
	QVector<NodeModel*> m_nodes;
    QVector<NodeConnection*> m_connections;
    NodeGraphController* m_controller;
};