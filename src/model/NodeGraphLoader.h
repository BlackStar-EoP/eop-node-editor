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

    static QJsonObject save(const NodeGraph& graph);
    bool load(const QJsonObject& json_data);

    QString last_error() const;

private:
    bool parse_node(const QJsonObject& node, QMap<uint32_t, NodeModel*>& node_models);

    NodeGraph& m_graph;
    NodeGraphController& m_controller;
    NodeFactory& m_factory;
    QString m_last_error;
};
