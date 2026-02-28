#include "NodeModel.h"

#include "NodePortModel.h"
#include <controllers/NodeGraphController.h>

#include <QQueue>

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

const QVector<NodePortModel*> NodeModel::input_ports() const
{
    return m_input_port_models;
}

QVector<NodeModel*> NodeModel::get_input_nodes() const
{
    QVector<NodeModel*> input_nodes;
    for (NodePortModel* input_port : m_input_port_models)
    {
        for (NodeConnection* connection : input_port->connections())
        {
            input_nodes.append(connection->output()->node_model());
        }
    }
    return input_nodes;
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

bool NodeModel::is_key_node() const
{
    return false;
}

QVector<KeyNode> NodeModel::find_ordered_key_nodes()
{
    class KeyNodeAdjacency
    {
    public:
        // Number of key nodes depending on this key node as input
        int downstream_node_count_ = 0;

        // Neighboring key nodes providing input to this node
        QVector<NodeModel*> upstream_nodes_;
    };
    class TraversalItem
    {
    public:
        TraversalItem(NodeModel* node, NodeModel* downstream_node = nullptr)
            : node_(node), downstream_node_(downstream_node)
        {
        }

        NodeModel* node_ = nullptr;
        NodeModel* downstream_node_ = nullptr;
    };

    // First determine the adjacency of every key node
    QHash<NodeModel*, KeyNodeAdjacency> adjacency_map;
    QQueue<TraversalItem> traversal_queue;

    traversal_queue.emplaceBack(this, nullptr);

    while (!traversal_queue.isEmpty())
    {
        NodeModel* node = traversal_queue.front().node_;
        NodeModel* downstream_pass = traversal_queue.front().downstream_node_;
        traversal_queue.dequeue();

        // Count in degrees and track neighbors only for these nodes
        if (node->is_key_node())
        {
            // Ensures node is created in the adjacency map
            KeyNodeAdjacency& info = adjacency_map[node];
            if (downstream_pass != nullptr)
            {
                info.downstream_node_count_++;

                KeyNodeAdjacency& downstream_info = adjacency_map[downstream_pass];
                downstream_info.upstream_nodes_.append(node);
            }
            // This is now the parent for all connected nodes
            downstream_pass = node;
        }

        // Enqueue all connected nodes
        for (NodePortModel* port_model : node->input_ports())
        {
            for (NodeConnection* connection : port_model->connections())
            {
                NodeModel* previous = connection->output()->node_model();
                traversal_queue.emplaceBack(previous, downstream_pass);
            }
        }
    }

    // Now order the the key nodes such that the dependencies are met
    // Start with the nodes with 0 incoming connections
    QQueue<NodeModel*> ready_queue;
    for (const auto& [node, adjaceny] : adjacency_map.asKeyValueRange())
    {
        if (adjaceny.downstream_node_count_ == 0)
        {
            ready_queue.enqueue(node);
        }

    }

    QVector<KeyNode> topological_order;
    topological_order.reserve(adjacency_map.size());
    while (!ready_queue.isEmpty())
    {
        NodeModel* node = ready_queue.dequeue();
        KeyNode node_info(node);
        topological_order.prepend(node_info);
        for (NodeModel* neighbor : adjacency_map[node].upstream_nodes_)
        {
            topological_order.first().upstream_nodes_.append(neighbor);

            KeyNodeAdjacency& adjacency = adjacency_map[neighbor];
            adjacency.downstream_node_count_--;
            if (adjacency.downstream_node_count_ == 0)
            {
                ready_queue.enqueue(neighbor);
            }
        }

    }
    return topological_order;
}