#include "materialnodemodel.h"

#include "material.h"

#include <src/model/NodePortModel.h>

MaterialNodeModel::MaterialNodeModel(Material* material)
: m_material(material)
{

}

void MaterialNodeModel::create_input_port_models()
{
	QVector<MaterialUniform> uniforms;
	m_material->getUniformsSortedByTypeAndName(uniforms);
	for (MaterialUniform& uniform : uniforms)
	{
		switch (uniform.m_type)
		{
			case MaterialUniform::FLOAT: add_input_port_model(new FloatPortModel(uniform.m_name)); break;
			case MaterialUniform::VEC2: add_input_port_model(new Vec2PortModel(uniform.m_name)); break;
			case MaterialUniform::VEC3: add_input_port_model(new Vec3PortModel(uniform.m_name)); break;
			case MaterialUniform::VEC4: add_input_port_model(new Vec4PortModel(uniform.m_name)); break;
			case MaterialUniform::SAMPLER_2D: add_input_port_model(new TexturePortModel(uniform.m_name)); break;
		}
	}
}

void MaterialNodeModel::create_output_port_models()
{
	const QVector<ShaderOutput>& outputs = m_material->outputs();
	for (const ShaderOutput& output : outputs)
	{
		add_output_port_model(new ShaderOutputPortModel(output.toString())); break;
	}
}

QString MaterialNodeModel::title() const
{
	return m_material->toString();
}

void MaterialNodeModel::on_connection(NodeConnection::EDirection direction, NodeModel* target_model, NodePortModel* target_port_model)
{

}

Material* MaterialNodeModel::material() const
{
	return m_material;
}