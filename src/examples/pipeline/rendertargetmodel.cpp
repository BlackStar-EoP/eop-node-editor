#include "rendertargetmodel.h"

#include "material.h"

#include "materialnodemodel.h"

#include <src/model/NodePortModel.h>

RenderTargetNodeModel::RenderTargetNodeModel(RenderTarget* target)
: m_target(target)
{

}

void RenderTargetNodeModel::create_input_port_models()
{
	add_input_port_model(new RenderTargetInputPortModel("Shader input"));
}

void RenderTargetNodeModel::create_output_port_models()
{
	if (m_material != nullptr)
	{
		const QVector<ShaderOutput>& textures = m_material->outputs();
		for (const ShaderOutput& output : textures)
		{
			add_output_port_model(new RenderTargetTexturePortModel(output.toString()));
		}
	}
}

QString RenderTargetNodeModel::title() const
{
	return "RenderTarget";
}

void RenderTargetNodeModel::on_connection(NodeConnection::EDirection direction, NodeModel* target_model, NodePortModel* target_port_model)
{
	if (direction == NodeConnection::INCOMING)
	{
		MaterialNodeModel* material_node_model = dynamic_cast<MaterialNodeModel*>(target_model);
		if (material_node_model != nullptr)
		{
			m_material = material_node_model->material();
			destroy_output_port_models();
			create_output_port_models();
			apply_node_model_to_ports_hack(); // TODO this needs a better solution
			output_nodes_changed();
		}
	}
}