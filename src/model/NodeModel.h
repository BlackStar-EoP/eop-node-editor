#pragma once

#include <QObject>
#include <QVector>

class NodePortModel;
class QPointF;

class INodeModelListener
{
public:
	virtual void node_model_changed() = 0;
};

class NodeModel : public QObject
{
	Q_OBJECT
public:

	virtual ~NodeModel();
	virtual void create_port_models() = 0;
	virtual void destroy_port_models() = 0;

	virtual QString title() const = 0;
	virtual uint32_t num_ports() const = 0;
	virtual NodePortModel* port_model(uint32_t port_nr) = 0;
	virtual void on_connection(NodeModel* target_model, NodePortModel* target_port_model) = 0;

	void node_model_changed();
	void register_node_model_listener(INodeModelListener* listener);

public:
	void set_position(const QPointF& position);
	const QPointF& position() const;
	bool is_orphan() const;

signals:
	void node_model_destroyed();

private:
	QVector<INodeModelListener*> m_node_model_listeners;
	QPointF m_position;
};