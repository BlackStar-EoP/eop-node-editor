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

	QJsonArray nodes_json = json_data["nodes"].toArray();
	QJsonArray connections_json = json_data["connections"].toArray();
	QMap<uint32_t, NodeModel*> node_models;

	QVector<QJsonObject> failed_nodes_json;
	for (int32_t i = 0; i < nodes_json.size(); ++i)
	{
		QJsonObject node = nodes_json[i].toObject();
		if (!parse_node(node, node_models))
			failed_nodes_json.push_back(node);
	}

	for (;;)
	{
		bool nodes_changed = false;
		for (int32_t i = failed_nodes_json.size() - 1; i >= 0; --i)
		{

			if (parse_node(failed_nodes_json[i], node_models))
			{
				nodes_changed = true;
				failed_nodes_json.removeAt(i);
			}
		}

		if (failed_nodes_json.size() == 0)
			break;

		if (!nodes_changed)
		{
            m_last_error = QString("Warning, %1 node(s) could not be created!").arg(failed_nodes_json.size());
            return false;
		}
	}

	for (;;)
	{
		bool connections_changed = false;
		for (int32_t i = connections_json.size() - 1; i >= 0; --i)
		{
			QJsonObject json_connection = connections_json[i].toObject();
			uint32_t input_model_id = json_connection["input_model_id"].toInt();
			uint32_t output_model_id = json_connection["output_model_id"].toInt();
			uint32_t input_port_index = json_connection["input_port_index"].toInt();
			uint32_t output_port_index = json_connection["output_port_index"].toInt();

			NodeModel* input_model = node_models[input_model_id];
			NodeModel* output_model = node_models[output_model_id];
			if (input_port_index < input_model->num_input_ports() && output_port_index < output_model->num_output_ports())
			{
				NodePortModel* input_port_model = input_model->input_port_model(input_port_index);
				NodePortModel* output_port_model = output_model->output_port_model(output_port_index);
				m_controller.set_first_connection_port(input_port_model);
				m_controller.set_second_connection_port(output_port_model);
				NodeConnection* connection = m_controller.create_connection();
				assert(connection != nullptr);
				connections_json.removeAt(i);
				connections_changed = true;
			}
		}

		if (connections_json.size() == 0)
			break;

		if (!connections_changed)
		{
            m_last_error = QString("Warning, %1 connection(s) could not be created!").arg(connections_json.size());
            return false;
		}
	}

    m_controller.end_load();
    return true;  // or false if loading failed
}

bool NodeGraphLoader::parse_node(const QJsonObject& node, QMap<uint32_t, NodeModel*>& node_models)
{
	uint32_t id = node["id"].toInt();
	QJsonObject node_data = node["node_data"].toObject();

	float x_pos = node_data["pos_x"].toDouble();
	float y_pos = node_data["pos_y"].toDouble();
	QString node_type = node_data["node_type"].toString();
	QJsonObject user_data = node_data["user_data"].toObject();

	m_factory.set_current_node_type(node_type);
	NodeModel* node_model = m_controller.add_node(QPoint(x_pos, y_pos));
	assert(node_model != nullptr);
	bool user_data_success = node_model->load_from_user_data(user_data);
	node_models[id] = node_model;
	return user_data_success;
}
