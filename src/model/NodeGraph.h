#pragma once

#include "Node.h"
#include <QVector>

class NodeGraph
{
public:
	NodeGraph();
	~NodeGraph();

private:
	QVector<Node> m_nodes;
};