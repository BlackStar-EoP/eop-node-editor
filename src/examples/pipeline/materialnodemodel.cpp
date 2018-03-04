#include "materialnodemodel.h"

#include "material.h"

#include <src/model/NodePortModel.h>

MaterialNodeModel::MaterialNodeModel(Material* material)
: m_material(material)
{

}

void MaterialNodeModel::create_port_models()
{
	QVector<MaterialUniform> uniforms;
	m_material->getUniformsSortedByTypeAndName(uniforms);
	for (MaterialUniform& uniform : uniforms)
	{
		switch (uniform.m_type)
		{
			case MaterialUniform::FLOAT: m_port_models.push_back(new FloatPortModel(uniform.m_name)); break;
			case MaterialUniform::VEC2: m_port_models.push_back(new Vec2PortModel(uniform.m_name)); break;
			case MaterialUniform::VEC3: m_port_models.push_back(new Vec3PortModel(uniform.m_name)); break;
			case MaterialUniform::VEC4: m_port_models.push_back(new Vec4PortModel(uniform.m_name)); break;
			case MaterialUniform::SAMPLER_2D: m_port_models.push_back(new TexturePortModel(uniform.m_name)); break;
		}
	}

	const QVector<ShaderOutput>& outputs = m_material->outputs();
	for (const ShaderOutput& output : outputs)
	{
		m_port_models.push_back(new ShaderOutputPortModel(output.toString())); break;
	}
}

void MaterialNodeModel::destroy_port_models()
{
	qDeleteAll(m_port_models);
}

QString MaterialNodeModel::title() const
{
	return m_material->toString();
}

uint32_t MaterialNodeModel::num_ports() const
{
	return m_port_models.size();
}

NodePortModel* MaterialNodeModel::port_model(uint32_t port_nr)
{
	if (num_ports() > port_nr)
		return m_port_models[port_nr];

	return nullptr;
}

void MaterialNodeModel::on_connection(NodeModel* target_model, NodePortModel* target_port_model)
{

}

Material* MaterialNodeModel::material() const
{
	return m_material;
}