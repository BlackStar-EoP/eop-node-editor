#pragma once

#include <src/model/NodeFactory.h>

class PipeLineNodeFactory : public NodeFactory
{
public:
	PipeLineNodeFactory();
	virtual NodeModel* create_node_model() override;
};