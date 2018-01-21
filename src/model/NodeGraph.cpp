#include "NodeGraph.h"

#include "model/Node.h"
#include "model/NodeConnection.h"

#include <assert.h>

NodeGraph::NodeGraph()
{
}

NodeGraph::~NodeGraph()
{
}

void NodeGraph::give_node(Node* node)
{
	m_nodes.push_back(node);
}

void NodeGraph::give_connection(NodeConnection* connection)
{
	assert(connection);
	m_connections.push_back(connection);
}