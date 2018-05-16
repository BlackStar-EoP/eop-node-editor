#include "NodeGraphWidget.h"

#include "NodeGraphScene.h"

#include <model/NodeFactory.h>
#include <model/NodeModel.h>
#include <model/NodePortModel.h>

#include <QMap>
#include <QSet>
#include <QBoxLayout>
#include <QGraphicsView>
#include <QJsonArray>

#include <assert.h>

NodeGraphWidget::NodeGraphWidget(QWidget* parent)
: QWidget(parent)
, m_controller(m_node_graph)
{
	m_scene = new NodeGraphScene(this, m_controller);
	QBoxLayout* layout = new QBoxLayout(QBoxLayout::Down);
	m_view = new QGraphicsView(m_scene);
	layout->addWidget(m_view);

	setLayout(layout);

	connect(&m_controller, SIGNAL(node_graph_changed()), this, SIGNAL(node_graph_changed()));
	connect(&m_controller, SIGNAL(message(const QString&)), this, SIGNAL(message(const QString&)));
}

NodeGraphWidget::~NodeGraphWidget()
{
	m_node_graph.clear();
	delete m_node_factory;
}

void NodeGraphWidget::give_node_factory(NodeFactory* factory)
{
	m_controller.set_node_factory(factory);
	m_node_factory = factory;
}

const QVector<NodeModel*>& NodeGraphWidget::nodes() const
{
	return m_node_graph.nodes();
}

void NodeGraphWidget::set_persisted()
{
	m_controller.set_persisted();
}

bool NodeGraphWidget::is_persisted() const
{
	return m_controller.is_persisted();
}

void NodeGraphWidget::new_graph()
{
	m_controller.clear_graph();
	//m_scene.clear_line_edit()
	m_node_graph.clear();
}

QJsonObject NodeGraphWidget::save_graph() const
{
	const QVector<NodeModel*>& node_vector = nodes();
	
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

void NodeGraphWidget::load_graph(const QJsonObject& json_data)
{
	new_graph();

	QJsonArray nodes_json = json_data["nodes"].toArray();
	QJsonArray connections_json = json_data["connections"].toArray();
	QMap<uint32_t, NodeModel*> node_models;

	for (int32_t i = 0; i < nodes_json.size(); ++i)
	{
		QJsonObject node = nodes_json[i].toObject();
		uint32_t id = node["id"].toInt();
		QJsonObject node_data = node["node_data"].toObject();

		float x_pos = node_data["pos_x"].toDouble();
		float y_pos = node_data["pos_y"].toDouble();
		QString node_type = node_data["node_type"].toString();
		QJsonObject user_data = node_data["user_data"].toObject();

		m_node_factory->set_current_node_type(node_type);
		NodeModel* node_model = m_controller.add_node(QPoint(x_pos, y_pos));
		assert(node_model != nullptr);
		node_model->load_from_user_data(user_data);
		node_models[id] = node_model;
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
			assert(false);
			break;
		}
	}

	set_persisted();
}