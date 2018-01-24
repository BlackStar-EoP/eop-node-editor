#include "rendertargetmodel.h"

#include "material.h"

#include <src/model/NodePort.h>

RenderTargetNodeModel::RenderTargetNodeModel(RenderTarget* target)
: m_target(target)
{

}

void RenderTargetNodeModel::create_port_models()
{
	//QVector<MaterialUniform> uniforms;
	//m_material->getUniformsSortedByTypeAndName(uniforms);
	//for (MaterialUniform& uniform : uniforms)
	//{
	//	switch (uniform.m_type)
	//	{
	//	case MaterialUniform::FLOAT: m_port_models.push_back(new FloatPortModel(uniform.m_name)); break;
	//	case MaterialUniform::VEC2: m_port_models.push_back(new Vec2PortModel(uniform.m_name)); break;
	//	case MaterialUniform::VEC3: m_port_models.push_back(new Vec3PortModel(uniform.m_name)); break;
	//	case MaterialUniform::VEC4: m_port_models.push_back(new Vec4PortModel(uniform.m_name)); break;
	//	case MaterialUniform::SAMPLER_2D: m_port_models.push_back(new TexturePortModel(uniform.m_name)); break;
	//	}
	//}

	//const QVector<ShaderOutput>& outputs = m_material->outputs();
	//for (const ShaderOutput& output : outputs)
	//{
	//	m_port_models.push_back(new ShaderOutputPortModel(output.toString())); break;
	//}
}

void RenderTargetNodeModel::destroy_port_models()
{
	//qDeleteAll(m_port_models);
}

QString RenderTargetNodeModel::title() const
{
	//return m_material->toString();
	return "";
}

uint32_t RenderTargetNodeModel::num_ports() const
{
	return 0;
	//return m_port_models.size();
}

NodePortModel* RenderTargetNodeModel::port_model(uint32_t port_nr)
{
	//if (num_ports() > port_nr)
	//	return m_port_models[port_nr];

	return nullptr;
}