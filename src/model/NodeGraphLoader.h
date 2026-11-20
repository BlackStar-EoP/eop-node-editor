#pragma once

#include <QJsonObject>
#include <QMap>
#include <QString>
#include <QUuid>
#include <stdexcept>

#include "controllers/NodeGraphController.h"

class NodeGraph;
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
    NodeGraphLoader(NodeGraph& graph, NodeFactory& factory);
    NodeGraphLoader(NodeGraph& graph, NodeGraphController& controller, NodeFactory& factory);

    /**
     * @throws LoadFailure
     */
    void load(const QJsonObject& json_data);

    static QJsonObject save(const NodeGraph& graph);

private:
    QMap<QUuid, NodeModel*> load_nodes(const QJsonObject& json_data);
    NodeModel* create_node(const QUuid& uuid, const QJsonObject& node_data);
    void load_connections(const QJsonObject& json_data, QMap<QUuid, NodeModel*> node_models);
    bool create_connection(NodePortModel* input, NodePortModel* output);
    void load_node_ports(const QJsonObject& json_data, QMap<QUuid, NodeModel*> node_models);

    std::unique_ptr<NodeGraphController> m_default_controller;
    NodeGraph& m_graph;
    NodeGraphController& m_controller;
    NodeFactory& m_factory;
};