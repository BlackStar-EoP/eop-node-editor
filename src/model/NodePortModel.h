#pragma once

#include <stdint.h>
#include <QObject>
#include <QString>
#include <QVector>

class NodeConnection;
class NodeModel;

class NodePortModel : public QObject
{
	Q_OBJECT
public:
	enum EPortType : int32_t
	{
		INPUT,
		OUTPUT
	};

	virtual ~NodePortModel();
	virtual EPortType port_type() const = 0;
	virtual QString port_label() const = 0;
	virtual QString name() = 0;
	virtual bool may_connect_to(const NodePortModel& port_model) const = 0;
	virtual bool supports_multiple_connections() const = 0;
	virtual QString type() const = 0;

signals:
	void node_port_model_destroyed();

public:
	void set_widget(QWidget* widget);
	QWidget* widget() const;
	void set_node_model(NodeModel* node_model);

	NodeModel* node_model() const;

	NodeConnection* connection(uint32_t index) const;
	uint32_t num_connections() const;
	void add_connection(NodeConnection* connection);
	void remove_connection(NodeConnection* connection);
	bool has_connection(NodePortModel* other_port_model);

private:
	NodeModel* m_node_model = nullptr;
	QVector<NodeConnection*> m_connections;
	QWidget* m_widget = nullptr;
};