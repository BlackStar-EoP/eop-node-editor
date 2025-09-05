#include "NodeModel.h"

#include "NodePortModel.h"
#include <controllers/NodeGraphController.h>

NodeModel::~NodeModel()
{
	for (NodePortModel* input_port : m_input_port_models)
	{
		delete input_port;
	}

	for (NodePortModel* output_port : m_output_port_models)
	{
		delete output_port;
	}

	emit node_model_destroyed();
}

QJsonObject NodeModel::to_json() const
{
	QJsonObject node_json;
	node_json["pos_x"] = m_position.x();
	node_json["pos_y"] = m_position.y();
	node_json["node_type"] = m_node_type.node_type();
	node_json["user_data"] = user_data();
	return node_json;
}

const NodeType& NodeModel::node_type() const
{
	return m_node_type;
}

void NodeModel::create_port_models()
{
	create_input_port_models();
	create_output_port_models();
	apply_node_model_to_ports_hack();
}

void NodeModel::apply_node_model_to_ports_hack()
{
	for (NodePortModel* port_model : m_input_port_models)
	{
		port_model->set_node_model(this);
	}

	for (NodePortModel* port_model : m_output_port_models)
	{
		port_model->set_node_model(this);
	}
}

uint32_t NodeModel::num_input_ports() const
{
	return m_input_port_models.size();
}

NodePortModel* NodeModel::input_port_model(uint32_t port_nr)
{
	if (num_input_ports() > port_nr)
		return m_input_port_models[port_nr];

	return nullptr;
}

void NodeModel::add_input_port_model(NodePortModel* port_model)
{
	m_input_port_models.push_back(port_model);
}

void NodeModel::destroy_input_port_models()
{
	for (NodePortModel* model : m_input_port_models)
	{
		delete model;
	}
	m_input_port_models.clear();
}

int32_t NodeModel::input_port_nr(NodePortModel* port_model) const
{
	return m_input_port_models.indexOf(port_model);
}

uint32_t NodeModel::num_output_ports() const
{
	return m_output_port_models.size();
}

NodePortModel* NodeModel::output_port_model(uint32_t port_nr)
{
	if (num_output_ports() > port_nr)
		return m_output_port_models[port_nr];

	return nullptr;
}

void NodeModel::add_output_port_model(NodePortModel* port_model)
{
	m_output_port_models.push_back(port_model);
}

void NodeModel::destroy_output_port_models()
{
	for (NodePortModel* model : m_output_port_models)
	{
		delete model;
	}

	m_output_port_models.clear();
}

int32_t NodeModel::output_port_nr(NodePortModel* port_model) const
{
	return m_output_port_models.indexOf(port_model);
}

uint32_t NodeModel::num_ports() const
{
	return m_input_port_models.size() + m_output_port_models.size();
}

void NodeModel::node_property_changed()
{
	// TODO maybe do this with the model listeners, I'm not sure yet
	m_controller->notify_node_graph_changed();
}

void NodeModel::node_model_changed()
{
	for (INodeModelListener* l : m_node_model_listeners)
	{
		l->node_model_changed();
	}
}

void NodeModel::input_nodes_changed()
{
    for (INodeModelListener* l : m_node_model_listeners)
    {
        l->input_nodes_changed();
    }
}

void NodeModel::output_nodes_changed()
{
	for (INodeModelListener* l : m_node_model_listeners)
	{
		l->output_nodes_changed();
	}
}

void NodeModel::register_node_model_listener(INodeModelListener* listener)
{
	m_node_model_listeners.push_back(listener);
}

void NodeModel::set_position(const QPointF& position)
{
	m_position = position;
}

void NodeModel::set_controller(NodeGraphController* controller)
{
	m_controller = controller;
}

const QPointF& NodeModel::position() const
{
	return m_position;
}

bool NodeModel::is_orphan() const
{
	for (NodePortModel* input : m_input_port_models)
	{
		if (input->num_connections() > 0)
			return false;
	}

	for (NodePortModel* output : m_output_port_models)
	{
		if (output->num_connections() > 0)
			return false;
	}

	return true;
}

void NodeModel::set_widget(QWidget* widget)
{
	m_widget = widget;
	// TODO model_changed()?
}

QWidget* NodeModel::widget() const
{
	return m_widget;
}

void NodeModel::set_node_type(const NodeType& node_type)
{
	m_node_type = node_type;
}