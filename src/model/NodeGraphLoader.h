#pragma once

#include <QJsonObject>
#include <QMap>
#include <QString>
#include <functional>

class NodeGraph;
class NodeGraphController;
class NodeFactory;
class NodeModel;
class NodePortModel;

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
    static bool load_impl(
            const QJsonObject& json_data,
            const std::function<NodeModel*(uint32_t id, const QJsonObject& node_data)>& create_node,
            const std::function<bool(NodePortModel* input, NodePortModel* output)>& create_connection,
            QString* out_error = nullptr
            );

    NodeGraph& m_graph;
    NodeGraphController& m_controller;
    NodeFactory& m_factory;
    QString m_last_error;
};