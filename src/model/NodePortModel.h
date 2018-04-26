#pragma once

#include <stdint.h>
#include <QObject>
#include <QString>

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
	void set_node_model(NodeModel* node_model);

	NodeModel* node_model() const;

	NodeConnection* connection() const;
	void set_connection(NodeConnection* connection);
	void remove_connection();

private:
	NodeModel* m_node_model = nullptr;
	NodeConnection* m_connection = nullptr;
};