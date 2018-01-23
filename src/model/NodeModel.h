#pragma once

#include <QVector>

class NodePort;
class NodePortModel;

class NodeModel
{
public:
	virtual ~NodeModel() = default;
	virtual void create_port_models() = 0;
	virtual void destroy_port_models() = 0;

	virtual QString title() const = 0;
	virtual uint32_t num_ports() const = 0;
	virtual NodePortModel* port_model(uint32_t port_nr) = 0;
};