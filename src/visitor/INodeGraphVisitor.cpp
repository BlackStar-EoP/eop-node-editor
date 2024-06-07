#include "INodeGraphVisitor.h"

#include <model/NodeModel.h>
#include <model/NodePortModel.h>

void INodeGraphVisitor::visit(NodeModel* node_model)
{
	Q_UNUSED(node_model);
}

void INodeGraphVisitor::visit(NodePortModel* node_port_model)
{
	Q_UNUSED(node_port_model);
}
