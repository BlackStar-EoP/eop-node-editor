#pragma once

class NodeModel;
class NodePortModel;

class INodeGraphVisitor
{
public:
	void visit(NodeModel* node_model);
	void visit(NodePortModel* node_port_model);
};
