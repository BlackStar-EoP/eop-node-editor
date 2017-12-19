#pragma once

#include <QVector>

#include "NodePort.h"

class Node
{
public:
	Node(const QPointF& position);
	~Node();

public:
	void add_input(const QString& port_label);
	void add_output(const QString& port_label);

	const QPointF& position() const;
	const QVector<NodePort>& input_ports() const;
	const QVector<NodePort>& output_ports() const;
	uint32_t num_ports() const;

private:
	QPointF m_position;
	QVector<NodePort> m_input_ports;
	QVector<NodePort> m_output_ports;
};