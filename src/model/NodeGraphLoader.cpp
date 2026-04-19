#include "NodeGraphLoader.h"

#include "NodeGraph.h"
#include "NodeFactory.h"
#include "NodeModel.h"
#include "NodePortModel.h"

#include <controllers/NodeGraphController.h>

#include <QJsonArray>
#include <QSet>

NodeGraphLoader::NodeGraphLoader(NodeGraph& graph, NodeGraphController& controller, NodeFactory& factory)
: m_graph(graph)
, m_controller(controller)
, m_factory(factory)
{
}

QString NodeGraphLoader::last_error() const
{
    return m_last_error;
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
		uint32_t input_model_id = node_model_map[input->node_model()];
		uint32_t output_model_id = node_model_map[output->node_model()];
		int32_t input_port_index = input->node_model()->input_port_nr(input);
		int32_t output_port_index = output->node_model()->output_port_nr(output);

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

bool NodeGraphLoader::load(const QJsonObject& json_data)
{
    m_controller.start_load();
    m_controller.clear_graph();
    m_graph.clear();

    const bool result = load_impl(
            json_data,
            [this](uint32_t id, const QJsonObject& node_data) -> NodeModel*
            {
                Q_UNUSED(id);
                const float x_pos = node_data["pos_x"].toDouble();
                const float y_pos = node_data["pos_y"].toDouble();
                const QString node_type = node_data["node_type"].toString();
                const QJsonObject user_data = node_data["user_data"].toObject();

                m_factory.set_current_node_type(node_type);
                NodeModel* node_model = m_controller.add_node(QPoint(x_pos, y_pos));
                assert(node_model != nullptr);
                return node_model;
            },
            [this](NodePortModel* input, NodePortModel* output) -> bool
            {
				m_controller.set_first_connection_port(input);
				m_controller.set_second_connection_port(output);
				NodeConnection* connection = m_controller.create_connection();
				return connection != nullptr;
            }
            );

    m_controller.end_load();
    return result;
}

bool NodeGraphLoader::load_graph(NodeGraph& graph, NodeFactory& factory, const QJsonObject& json_data)
{
    graph.clear();

    return load_impl(
            json_data,
            [&graph, &factory](uint32_t id, const QJsonObject& node_data) -> NodeModel*
            {
                Q_UNUSED(id);
                factory.set_current_node_type(node_data["node_type"].toString());
                NodeModel* model = factory.create_node_model_and_set_type();
                assert(model != nullptr);
                graph.give_node(model);
                model->set_position(QPointF(node_data["pos_x"].toDouble(),
                                            node_data["pos_y"].toDouble()));
                model->create_port_models();
                return model;
            },
            [](NodePortModel* input, NodePortModel* output) -> bool
            {
                new NodeConnection(input, output);
                return true;
            }
            );
}

bool NodeGraphLoader::load_impl(
            const QJsonObject& json_data,
            const std::function<NodeModel*(uint32_t id, const QJsonObject& node_data)>& create_node,
            const std::function<bool(NodePortModel* input, NodePortModel* output)>& create_connection,
            QString* out_error
            )
{
    Q_UNUSED(out_error);

    QJsonArray nodes_json = json_data["nodes"].toArray();
    QJsonArray connections_json = json_data["connections"].toArray();
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

        NodeModel* model = create_node(id, node_data);
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
            return false;
    }

    QJsonArray pending_connections = connections_json;
    for (;;)
    {
        bool progress = false;

        for (int32_t i = pending_connections.size() - 1; i >= 0; --i)
        {
            QJsonObject json_conn   = pending_connections[i].toObject();
            uint32_t input_id       = json_conn["input_model_id"].toInt();
            uint32_t output_id      = json_conn["output_model_id"].toInt();
            uint32_t input_port_idx  = json_conn["input_port_index"].toInt();
            uint32_t output_port_idx = json_conn["output_port_index"].toInt();

            NodeModel* input_model  = node_models[input_id];
            NodeModel* output_model = node_models[output_id];

            if (input_port_idx  < input_model->num_input_ports() &&
                output_port_idx < output_model->num_output_ports())
            {
                if (create_connection(input_model->input_port_model(input_port_idx),
                                      output_model->output_port_model(output_port_idx)))
                {
                    pending_connections.removeAt(i);
                    progress = true;
                }
            }
        }

        if (pending_connections.isEmpty())
            break;
        if (!progress)
            return false;
    }

    return true;
}