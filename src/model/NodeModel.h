#pragma once

#include <QObject>
#include <QVector>

#include "NodeConnection.h"
#include "NodeType.h"

class NodePortModel;
class QPointF;

class INodeModelListener
{
public:
	virtual void node_model_changed() = 0;
	virtual void output_nodes_changed() = 0;
};

class NodeModel : public QObject
{
	Q_OBJECT
public:

	virtual ~NodeModel();
	virtual void create_input_port_models() = 0;
	virtual void create_output_port_models() = 0;
	virtual QString title() const = 0;
	const NodeType& node_type() const;
	
	void create_port_models();
	void apply_node_model_to_ports_hack();

	uint32_t num_input_ports() const;
	NodePortModel* input_port_model(uint32_t port_nr);
	void add_input_port_model(NodePortModel* port_model);
	void destroy_input_port_models();

	uint32_t num_output_ports() const;
	NodePortModel* output_port_model(uint32_t port_nr);
	void add_output_port_model(NodePortModel* port_model);
	void destroy_output_port_models();

	uint32_t num_ports() const;

	virtual void on_connection(NodeConnection::EDirection direction, NodeModel* target_model, NodePortModel* target_port_model) = 0;

	void node_model_changed();
	void output_nodes_changed();
	void register_node_model_listener(INodeModelListener* listener);

	void set_position(const QPointF& position);
	const QPointF& position() const;
	bool is_orphan() const;
	void set_widget(QWidget* widget);
	QWidget* widget() const;

	void set_node_type(const NodeType& node_type);

signals:
	void node_model_destroyed();

private:
	QVector<INodeModelListener*> m_node_model_listeners;
	QPointF m_position;

	QVector<NodePortModel*> m_input_port_models;
	QVector<NodePortModel*> m_output_port_models;
	QWidget* m_widget = nullptr;
	NodeType m_node_type;
};