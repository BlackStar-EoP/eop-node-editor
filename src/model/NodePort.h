#pragma once

#include "NodePortModel.h"
#include <stdint.h>

#include <QString>

class Node;
class NodeConnection;

class NodePort
{
public:

public:
	NodePort(NodePortModel* model, const Node* const node);
	~NodePort() = default;

	NodePortModel::EPortType port_type() const;
	QString port_label() const;
	const Node* const node() const;

	const NodeConnection* const connection() const;
	void set_connection(const NodeConnection* const connection);
	void remove_connection();
	bool may_connect_to(NodePort* port) const;

private:
	const NodePortModel* m_model = nullptr;
	const Node* const m_node = nullptr;
	const NodeConnection* m_connection = nullptr;

};