#pragma once

#include <QObject>
#include <QVector>
#include <QJsonObject>
#include <QPointF>

#include "NodeConnection.h"
#include "NodeType.h"
#include "NodePortModel.h"

class NodeGraphController;

class INodeModelListener
{
public:
	virtual void node_model_changed() = 0;
    virtual void input_nodes_changed() = 0;
	virtual void output_nodes_changed() = 0;
};

class KeyNode
{
public:
    KeyNode(NodeModel* node)
        : node_(node)
    {
    }

    NodeModel* node_ = nullptr;
    QVector<NodeModel*> upstream_nodes_;
};

class NodeModel : public QObject
{
	Q_OBJECT
public:

	virtual ~NodeModel();
	virtual void create_input_port_models() = 0;
	virtual void create_output_port_models() = 0;
	virtual QString title() const = 0;
	virtual QJsonObject user_data() const = 0;
	virtual bool load_from_user_data(const QJsonObject& user_data) = 0;

	QJsonObject to_json() const;

	const NodeType& node_type() const;

	void create_port_models();
	void apply_node_model_to_ports_hack();

	uint32_t num_input_ports() const;
	NodePortModel* input_port_model(uint32_t port_nr);
	void add_input_port_model(NodePortModel* port_model);
	void destroy_input_port_models();
	int32_t input_port_nr(NodePortModel* port_model) const;

    /**
     * All input ports for this node.
     */
    const QVector<NodePortModel*> input_ports() const;

    /**
     * Retrieve all nodes connected to an input port of the current node.
     */
    QVector<NodeModel*> get_input_nodes() const;

    /**
     * Retrieve the node of type `InputNodeType` connected to any input port of the current node.
     *
     * @pre Requires only one input port is connected to the node type and the input port to only accept a single connection.
     *
     * @returns nullptr if none found
     */
    template <class InputNodeType>
    InputNodeType* get_input_node_by_type() const
    {
        for (NodePortModel* port : m_input_port_models)
        {
            for (NodeConnection* connection : port->connections())
            {
                if (InputNodeType* node = qobject_cast<InputNodeType*>(connection->output()->node_model());
                        node != nullptr)
                {
                    assert(!port->supports_multiple_connections());
                    return node;
                }
            }
        }
        return nullptr;
    }

    /**
     * Retrieve the output port of type `OutputPortType` connected to any input of this node.
     *
     * @pre Requires only one input port is connected to the node type and the input port to only accept a single connection.
     *
     * @returns nullptr if none found
     */
    template <class OutputPortType>
    OutputPortType* get_input_node_output_port_by_type() const
    {
        for (NodePortModel* port : m_input_port_models)
        {
            for (NodeConnection* connection : port->connections())
            {
                if (OutputPortType* required_port = qobject_cast<OutputPortType*>(connection->output());
                        required_port != nullptr)
                {
                    // TODO: assert(!port->supports_multiple_connections());
                    return required_port;
                }
            }
        }
        return nullptr;
    }

	uint32_t num_output_ports() const;
	NodePortModel* output_port_model(uint32_t port_nr);
	void add_output_port_model(NodePortModel* port_model);
	void destroy_output_port_models();
	int32_t output_port_nr(NodePortModel* port_model) const;

    /**
     * All output ports for this node.
     */
    const QVector<NodePortModel*> output_ports() const;

    /**
     * Retrieve the connection on any output port towards a node of type `OutputNodeType`.
     *
     * @pre Requires only one output port is connected to the node type and the output port to only accept a single connection.
     *
     * @returns nullptr if none found
     */
    template <class OutputNodeType>
    NodeConnection* get_output_connection_by_node_type() const
    {
        for (NodePortModel* port : m_output_port_models)
        {
            for (NodeConnection* connection : port->connections())
            {
                if (qobject_cast<OutputNodeType*>(connection->input()->node_model()) != nullptr)
                {
                    // TODO: assert(!port->supports_multiple_connections());
                    return connection;
                }
            }
        }
        return nullptr;
    }

	uint32_t num_ports() const;

	virtual void connection_added(NodePortModel* port_model, NodeConnection* connection) = 0;
	virtual void connection_removed(NodePortModel* port_model, NodeConnection* connection) = 0;

	void node_model_changed();
	void node_property_changed();
    void input_nodes_changed();
	void output_nodes_changed();
	void register_node_model_listener(INodeModelListener* listener);

	void set_position(const QPointF& position);
	const QPointF& position() const;
	void set_controller(NodeGraphController* controller); // TODO I am not so sure about this, but still I want to be able to notify when a property changes
	bool is_orphan() const;
	void set_widget(QWidget* widget);
	QWidget* widget() const;

	void set_node_type(const NodeType& node_type);

    /**
     * Is this node model a "key node"?
     *
     * When traversing a graph, these key nodes get special treatment. E.g. they may result in new objects being
     * created and updated with information from connected nodes that are not key nodes.
     */
    virtual bool is_key_node() const;

    /**
     * Get an ordered list of nodes marked as key nodes (`is_key_node()`).
     *
     * Uses Kahn's algorithm to traverse the tree from an endpoint back to the beginnings. Results in a list
     * where every key node is after each key node it depends on. This ensures dependencies are always resolved
     * before moving to the next key node.
     *
     * Important: this method only traverses in the direction of the input ports/nodes. Calling it on a node
     * in the middle of the graph will result in a partial list.
     */
    QVector<KeyNode> find_ordered_key_nodes();

signals:
	void node_model_destroyed();

protected: // TODO make this private again, and make the json methods non-const.
	QVector<NodePortModel*> m_input_port_models;
	QVector<NodePortModel*> m_output_port_models;

private:
	QVector<INodeModelListener*> m_node_model_listeners;
	QPointF m_position;

	QWidget* m_widget = nullptr;
	NodeType m_node_type;
	NodeGraphController* m_controller = nullptr;
};