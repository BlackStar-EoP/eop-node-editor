#pragma once

class NodeModel;

class NodeFactory
{
public:
	NodeFactory() = default;
	virtual ~NodeFactory() = default;

	virtual NodeModel* create_node_model() = 0;

};