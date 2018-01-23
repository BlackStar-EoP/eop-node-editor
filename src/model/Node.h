#pragma once

#include <QVector>

#include "NodePort.h"

class NodeModel;

class Node
{
public:
	Node(const QPointF& position, NodeModel* model);
	~Node();

public:
	void set_position(const QPointF& position);
	const QString title() const;
	NodeModel* model() const;
	const QPointF& position() const;
	bool is_orphan() const;

private:
	QPointF m_position;
	NodeModel* m_model;
};