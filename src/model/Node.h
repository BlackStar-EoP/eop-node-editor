#pragma once

#include <QVector>

#include "NodePort.h"

class NodeModel;

class Node
{
public:
	Node(const QString& name, const QPointF& position, NodeModel* model);
	~Node();

public:
	void add_input(const QString& port_label);
	void add_output(const QString& port_label);

	void set_position(const QPointF& position);
	const QString& name() const;
	const QPointF& position() const;
	const QVector<NodePort*>& input_ports() const;
	const QVector<NodePort*>& output_ports() const;
	uint32_t num_ports() const;
	bool is_orphan() const;

private:
	QString m_name;
	QPointF m_position;
	NodeModel* m_model;
	QVector<NodePort*> m_input_ports;
	QVector<NodePort*> m_output_ports;
};