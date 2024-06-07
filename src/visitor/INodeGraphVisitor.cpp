#include "INodeGraphVisitor.h"

#include <model/NodeModel.h>
#include <model/NodePortModel.h>

#include <QDebug>

void INodeGraphVisitor::visit(NodeModel* node_model)
{
	qInfo() << "VISIT NodeModel " << node_model->node_type().node_type();
	for (uint32_t i = 0u; i < node_model->num_input_ports(); ++i)
	{
		node_model->input_port_model(i)->accept(*this);
	}

	for (uint32_t i = 0u; i < node_model->num_output_ports(); ++i)
	{
		node_model->input_port_model(i)->accept(*this);
	}

}

void INodeGraphVisitor::visit(NodePortModel* node_port_model)
{
	qInfo() << "VISIT NodePortModel " << node_port_model->type();
	for (uint32_t i = 0u; i < node_port_model->num_connections(); ++i)
	{
		node_port_model->connection(i)->input()->accept(*this);
		node_port_model->connection(i)->output()->accept(*this);
	}
}
