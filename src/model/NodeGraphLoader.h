#pragma once

#include <QJsonObject>
#include <QMap>
#include <QString>

class NodeGraph;
class NodeGraphController;
class NodeFactory;
class NodeModel;

class NodeGraphLoader
{
public:
    NodeGraphLoader(NodeGraph& graph, NodeGraphController& controller, NodeFactory& factory);

    bool load(const QJsonObject& json_data);

    /**
     * Load a graph from JSON without a controller for read-only usage without a UI.
     * No signals will be emitted while constructing the graph.
     */
    static bool load_graph(NodeGraph& graph, NodeFactory& factory, const QJsonObject& json_data);

    static QJsonObject save(const NodeGraph& graph);

    QString last_error() const;

private:
    bool parse_node(const QJsonObject& node, QMap<uint32_t, NodeModel*>& node_models);

    NodeGraph& m_graph;
    NodeGraphController& m_controller;
    NodeFactory& m_factory;
    QString m_last_error;
};