#include "NodeModel.h"

#include "NodePortModel.h"

NodeModel::~NodeModel()
{
	emit node_model_destroyed();
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
	qDeleteAll(m_input_port_models);
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
	qDeleteAll(m_output_port_models);
}

uint32_t NodeModel::num_ports() const
{
	return m_input_port_models.size() + m_output_port_models.size();
}

void NodeModel::node_model_changed()
{
	for (INodeModelListener* l : m_node_model_listeners)
	{
		l->node_model_changed();
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

const QPointF& NodeModel::position() const
{
	return m_position;
}

bool NodeModel::is_orphan() const
{
	//for (NodePort* input : m_input_ports)
	//{
	//	if (input->connection() != nullptr)
	//		return false;
	//}

	//for (NodePort* output : m_output_ports)
	//{
	//	if (output->connection() != nullptr)
	//		return false;
	//}

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