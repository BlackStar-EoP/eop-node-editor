#pragma once

#include <stdint.h>

#include <QString>

class Node;
class NodeConnection;

class NodePort
{
public:
	enum EPortType : int32_t
	{
		INPUT,
		OUTPUT
	};
public:
	NodePort() = default;
	NodePort(EPortType port_type, const QString& port_label, const Node* const node);
	~NodePort() = default;

	EPortType port_type() const;
	const QString& port_label() const;
	const Node* const node() const;

	const NodeConnection* const connection() const;
	void set_connection(const NodeConnection* const connection);
	void remove_connection();

private:
	EPortType m_port_type = INPUT;
	QString m_port_label;
	const Node* const m_node = nullptr;
	const NodeConnection* m_connection = nullptr;

};