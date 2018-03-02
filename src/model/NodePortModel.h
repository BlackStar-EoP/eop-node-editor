#pragma once

#include <stdint.h>
#include <QObject>
#include <QString>

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

signals:
	void node_port_model_destroyed();
};