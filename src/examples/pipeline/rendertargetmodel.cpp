#include "rendertargetmodel.h"

#include "material.h"

#include "materialnodemodel.h"

#include <src/model/NodePortModel.h>

RenderTargetNodeModel::RenderTargetNodeModel(RenderTarget* target)
: m_target(target)
{

}

void RenderTargetNodeModel::create_port_models()
{
	m_port_models.push_back(new RenderTargetInputPortModel("Shader input"));
	if (m_material != nullptr)
	{
		const QVector<ShaderOutput>& textures = m_material->outputs();
		for (const ShaderOutput& output : textures)
		{
			m_port_models.push_back(new RenderTargetTexturePortModel(output.toString()));
		}
	}
}

void RenderTargetNodeModel::destroy_port_models()
{
	qDeleteAll(m_port_models);
	m_port_models.clear();
}

QString RenderTargetNodeModel::title() const
{
	return "RenderTarget";
}

uint32_t RenderTargetNodeModel::num_ports() const
{
	return m_port_models.size();
}

NodePortModel* RenderTargetNodeModel::port_model(uint32_t port_nr)
{
	if (num_ports() > port_nr)
		return m_port_models[port_nr];

	return nullptr;
}

void RenderTargetNodeModel::on_connection(NodeModel* target_model, NodePortModel* target_port_model)
{
	MaterialNodeModel* material_node_model = dynamic_cast<MaterialNodeModel*>(target_model);
	if (material_node_model != nullptr)
	{
		m_material = material_node_model->material();
		for (NodePortModel* port_model : m_port_models)
		create_port_models();
		node_model_changed();
	}
}