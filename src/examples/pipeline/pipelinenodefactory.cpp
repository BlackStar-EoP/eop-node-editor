#include "pipelinenodefactory.h"

#include "materialnodemodel.h"
#include "material.h"
#include "rendertargetmodel.h"
#include "rendertarget.h"

PipeLineNodeFactory::PipeLineNodeFactory()
{
	register_node_type(MaterialNodeModel::TYPE_NAME);
	register_node_type(RenderTargetNodeModel::TYPE_NAME);
}

NodeModel* PipeLineNodeFactory::create_node_model()
{
	static int type = 0;
	++type;

	if (type % 2 == 0)
	{
		static int materialnr = 0;
		Material* material = new Material(materialnr++, "Material");

		uint32_t num_flt = rand() % 2 + 1;
		uint32_t num_vec2 = rand() % 2;
		uint32_t num_vec3 = rand() % 2;
		uint32_t num_vec4 = rand() % 2;
		uint32_t num_tex = rand() % 2;

		for (uint32_t i = 0; i < num_flt; ++i)
			material->addUniform(MaterialUniform(MaterialUniform::FLOAT, "Float var"));
		for (uint32_t i = 0; i < num_vec2; ++i)
			material->addUniform(MaterialUniform(MaterialUniform::VEC2, "Vec2 var"));
		for (uint32_t i = 0; i < num_vec3; ++i)
			material->addUniform(MaterialUniform(MaterialUniform::VEC3, "Vec3 var"));
		for (uint32_t i = 0; i < num_vec4; ++i)
			material->addUniform(MaterialUniform(MaterialUniform::VEC4, "Vec4 var"));
		for (uint32_t i = 0; i < num_tex; ++i)
			material->addUniform(MaterialUniform(MaterialUniform::SAMPLER_2D, "Texture"));

		uint32_t num_outputs = rand() % 2 + 1;
		for (uint32_t i = 0; i < num_outputs; ++i)
			material->addOutput(ShaderOutput(i, MaterialUniform::VEC4, "fragcolor"));

		return new MaterialNodeModel(material);
	}
	else
	{
		return new RenderTargetNodeModel(new RenderTarget());
	}
}