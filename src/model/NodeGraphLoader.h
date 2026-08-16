#pragma once

#include <QJsonObject>
#include <QMap>
#include <QString>
#include <functional>
#include <stdexcept>

class NodeGraph;
class NodeGraphController;
class NodeFactory;
class NodeModel;
class NodePortModel;

class NodeGraphLoader
{
public:
    class LoadFailure : public std::runtime_error
    {
    public:
        explicit LoadFailure(const QString& arg_what);
    };

public:
    NodeGraphLoader(NodeGraph& graph, NodeGraphController& controller, NodeFactory& factory);

    bool load(const QJsonObject& json_data);

    /**
     * Load a graph from JSON without a controller for read-only usage without a UI.
     * No signals will be emitted while constructing the graph.
     *
     * @throws LoadFailure
     */
    static void load_graph(NodeGraph& graph, NodeFactory& factory, const QJsonObject& json_data);

    static QJsonObject save(const NodeGraph& graph);

    QString last_error() const;

private:
    static void load_impl(
            const QJsonObject& json_data,
            const std::function<NodeModel*(uint32_t id, const QJsonObject& node_data)>& create_node,
            const std::function<bool(NodePortModel* input, NodePortModel* output)>& create_connection
            );

    NodeGraph& m_graph;
    NodeGraphController& m_controller;
    NodeFactory& m_factory;
    QString m_last_error;
};