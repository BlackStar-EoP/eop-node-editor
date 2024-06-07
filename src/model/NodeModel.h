#pragma once

#include <QObject>
#include <QVector>
#include <QJSonObject>
#include <QPointF>

#include "NodeConnection.h"
#include "NodeType.h"
#include "visitor/INodeGraphVisitor.h"

class NodePortModel;
class NodeGraphController;

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
	virtual QJsonObject user_data() const = 0;
	virtual bool load_from_user_data(const QJsonObject& user_data) = 0;
	virtual void accept(INodeGraphVisitor& visitor) = 0;

	QJsonObject to_json() const;

	const NodeType& node_type() const;
	
	void create_port_models();
	void apply_node_model_to_ports_hack();

	uint32_t num_input_ports() const;
	NodePortModel* input_port_model(uint32_t port_nr);
	void add_input_port_model(NodePortModel* port_model);
	void destroy_input_port_models();
	int32_t input_port_nr(NodePortModel* port_model) const;

	uint32_t num_output_ports() const;
	NodePortModel* output_port_model(uint32_t port_nr);
	void add_output_port_model(NodePortModel* port_model);
	void destroy_output_port_models();
	int32_t output_port_nr(NodePortModel* port_model) const;

	uint32_t num_ports() const;

	virtual void connection_added(NodePortModel* port_model, NodeConnection* connection) = 0;
	virtual void connection_removed(NodePortModel* port_model, NodeConnection* connection) = 0;

	void node_model_changed();
	void node_property_changed();
	void output_nodes_changed();
	void register_node_model_listener(INodeModelListener* listener);

	void set_position(const QPointF& position);
	const QPointF& position() const;
	void set_controller(NodeGraphController* controller); // TODO I am not so sure about this, but still I want to be able to notify when a property changes
	bool is_orphan() const;
	void set_widget(QWidget* widget);
	QWidget* widget() const;

	void set_node_type(const NodeType& node_type);

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