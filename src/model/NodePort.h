#pragma once

#include <stdint.h>

#include <QString>

class Node;

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

private:
	EPortType m_port_type = INPUT;
	QString m_port_label;
	const Node* const m_node = nullptr;
};