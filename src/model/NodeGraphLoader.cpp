#include "NodeGraphLoader.h"

#include "NodeGraph.h"
#include "NodeFactory.h"
#include "NodeModel.h"
#include "NodePortModel.h"

#include <controllers/NodeGraphController.h>

#include <QJsonArray>
#include <QSet>
#include <memory>

NodeGraphLoader::LoadFailure::LoadFailure(const QString& arg_what)
    : std::runtime_error(arg_what.toStdString())
{
}

NodeGraphLoader::NodeGraphLoader(NodeGraph& graph, NodeFactory& factory)
: m_default_controller(std::make_unique<NodeGraphController>(&graph))
, m_graph(graph)
, m_controller(*m_default_controller)
, m_factory(factory)
{
}

NodeGraphLoader::NodeGraphLoader(NodeGraph& graph, NodeGraphController& controller, NodeFactory& factory)
: m_graph(graph)
, m_controller(controller)
, m_factory(factory)
{
}

QJsonObject NodeGraphLoader::save(const NodeGraph& graph)
{
	const QVector<NodeModel*>& node_vector = graph.nodes();

	// Map all node models to an internal id
	QMap<NodeModel*, uint32_t> node_model_map;
	QSet<NodeConnection*> node_connections;

	uint32_t node_id = 1;
	for (NodeModel* node : node_vector)
	{
		node_model_map[node] = node_id++;
		// Store input connections
		for (uint32_t i = 0; i < node->num_input_ports(); ++i)
		{
			NodePortModel* input_port = node->input_port_model(i);
			for (uint32_t j = 0; j < input_port->num_connections(); ++j)
			{
				node_connections.insert(input_port->connection(j));
			}
		}

		// Store output connections
		for (uint32_t i = 0; i < node->num_output_ports(); ++i)
		{
			NodePortModel* output_port = node->output_port_model(i);
			for (uint32_t j = 0; j < output_port->num_connections(); ++j)
			{
				node_connections.insert(output_port->connection(j));
			}
		}

	}

	QJsonArray node_array;
	for (NodeModel* node : node_vector)
	{
		QJsonObject json_node;
		json_node["id"] = (qint64)node_model_map[node];
		json_node["node_data"] = node->to_json();
		node_array.push_back(json_node);
	}

	QJsonArray node_connection_array;
	for (NodeConnection* connection : node_connections)
	{
		QJsonObject json_connection;
		NodePortModel* input = connection->input();
		NodePortModel* output = connection->output();
		uint32_t input_model_id = node_model_map[input->exposing_node()];
		uint32_t output_model_id = node_model_map[output->exposing_node()];
		int32_t input_port_index = input->exposing_node()->input_port_nr(input);
		int32_t output_port_index = output->exposing_node()->output_port_nr(output);

		json_connection["input_model_id"] = (qint64) input_model_id;
		json_connection["output_model_id"] = (qint64) output_model_id;
		json_connection["input_port_index"] = (qint64) input_port_index;
		json_connection["output_port_index"] = (qint64) output_port_index;
		node_connection_array.push_back(json_connection);
	}

	QJsonObject node_graph;
	node_graph["nodes"] = node_array;
	node_graph["connections"] = node_connection_array;
	return node_graph;
}

void NodeGraphLoader::load(const QJsonObject& json_data)
{
    m_controller.start_load();
    m_controller.clear_graph();
    m_graph.clear();

    QMap<uint32_t, NodeModel*> node_models = load_nodes(json_data);
    load_connections(json_data, node_models);

    m_controller.end_load();
}

QMap<uint32_t, NodeModel*> NodeGraphLoader::load_nodes(const QJsonObject& json_data)
{
    QJsonArray nodes_json = json_data["nodes"].toArray();
    QMap<uint32_t, NodeModel*> node_models;

    // Create all node models in one pass, track those whose load_from_user_data
    // fails (e.g. forward references to objects not yet loaded).
    struct PendingNode { uint32_t id; QJsonObject user_data; };
    QVector<PendingNode> pending;

    for (int32_t i = 0; i < nodes_json.size(); ++i)
    {
        QJsonObject node_json   = nodes_json[i].toObject();
        uint32_t id             = node_json["id"].toInt();
        QJsonObject node_data   = node_json["node_data"].toObject();
        QJsonObject user_data   = node_data["user_data"].toObject();

        NodeModel* model = create_node(node_data);
        node_models[id] = model;

        if (!model->load_from_user_data(user_data))
            pending.push_back({id, user_data});
    }

    // Retry load_from_user_data for nodes that failed (forward references).
    // Only the data call is retried — the model is already in the graph.
    for (;;)
    {
        bool progress = false;
        for (int32_t i = pending.size() - 1; i >= 0; --i)
        {
            if (node_models[pending[i].id]->load_from_user_data(pending[i].user_data))
            {
                pending.removeAt(i);
                progress = true;
            }
        }
        if (pending.isEmpty())
            break;
        if (!progress)
        {
            throw LoadFailure(QString("Unable to load %1 nodes.").arg(pending.count()));
        }
    }

    return node_models;
}

NodeModel* NodeGraphLoader::create_node(const QJsonObject& node_data)
{
    m_factory.set_current_node_type(node_data["node_type"].toString());
    NodeModel* model = m_factory.create_node_model_and_set_type();
    assert(model != nullptr);
    m_graph.give_node(model);
    model->set_position(QPointF(node_data["pos_x"].toDouble(),
                                node_data["pos_y"].toDouble()));
    model->create_port_models();
    return model;
}

void NodeGraphLoader::load_connections(const QJsonObject& json_data, QMap<uint32_t, NodeModel*> node_models)
{
    QJsonArray connections_json = json_data["connections"].toArray();
    constexpr int MAX_CONNECTION_ATTEMPTS = 10;
    for (int attempt = 0;; attempt++)
    {
        bool progress = false;
        int connections_present = 0;

        for (QJsonValueRef json_conn_value : connections_json)
        {
            QJsonObject json_conn   = json_conn_value.toObject();
            uint32_t input_id       = json_conn["input_model_id"].toInt();
            uint32_t output_id      = json_conn["output_model_id"].toInt();
            uint32_t input_port_idx  = json_conn["input_port_index"].toInt();
            uint32_t output_port_idx = json_conn["output_port_index"].toInt();

            NodeModel* input_model  = node_models[input_id];
            NodeModel* output_model = node_models[output_id];

            if (input_port_idx  < input_model->num_input_ports() &&
                output_port_idx < output_model->num_output_ports())
            {
                NodePortModel* input_port_model = input_model->input_port_model(input_port_idx);
                NodePortModel* output_port_model = output_model->output_port_model(output_port_idx);
                if (input_port_model->has_connection(output_port_model))
                {
                    connections_present++;
                }
                else if (create_connection(input_port_model, output_port_model))
                {
                    connections_present++;
                    progress = true;
                }
            }
        }

        if (connections_present == connections_json.size())
            break;
        if (!progress || attempt > MAX_CONNECTION_ATTEMPTS)
        {
            throw LoadFailure(
                QString("Unable to load %1 connections.").arg(connections_json.size() - connections_present));
        }
    }
}

bool NodeGraphLoader::create_connection(NodePortModel* input, NodePortModel* output)
{
    m_controller.set_first_connection_port(input);
    m_controller.set_second_connection_port(output);
    return m_controller.create_connection() != nullptr;
}