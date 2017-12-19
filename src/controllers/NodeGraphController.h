#pragma once

class QPointF;

class Node;

class NodeGraphController
{
public:
	NodeGraphController() = default;
	~NodeGraphController() = default;

	Node* add_node(const QPointF& position);
};