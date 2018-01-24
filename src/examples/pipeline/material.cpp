#include "material.h"

#include <assert.h>

Material::Material(uint32_t id, const QString& name)
: id_(id)
, name_(name)
{
}

uint32_t Material::id() const
{
	return id_;
}

const QString& Material::name() const
{
	return name_;
}

void Material::setShaders(Shader* vertex, Shader* fragment, Shader* geom, Shader* tessControl, Shader* tessEval)
{
	vertexShader_ = vertex;
	fragmentShader_ = fragment;
	geometryShader_ = geom;
	tessControlShader_ = tessControl;
    tessEvalShader_ = tessEval;
}

void Material::setVertexShader(Shader *vertexShader)
{
    vertexShader_ =  vertexShader;
}

void Material::setFragmentShader(Shader *fragmentShader)
{
    fragmentShader_ = fragmentShader;
}

void Material::setGeomShader(Shader *geomShader)
{
    geometryShader_ = geomShader;
}

void Material::setTessControlShader(Shader *tessControlShader)
{
    tessControlShader_ = tessControlShader;
}

void Material::setTessEvalShader(Shader *tessEvalShader)
{
    tessEvalShader_ = tessEvalShader;
}

DeviseMaterialParameters* Material::createParameters()
{
	return nullptr;
}

void Material::setParameters(DeviseMaterialParameters& parameters)
{
}

const QVector<ShaderOutput>& Material::outputs() const
{
	return outputs_;
}

void Material::addOutput(const ShaderOutput& output)
{
	outputs_.push_back(output);
}


const QVector<MaterialUniform>& Material::uniforms() const
{
    return uniforms_;
}

void Material::getUniformsSortedByTypeAndName(QVector<MaterialUniform>& uniforms)
{
	uniforms = uniforms_;
	std::sort(uniforms.begin(), uniforms.end(),
		[](const MaterialUniform& u1, const MaterialUniform& u2)
		{
			return u1.m_type < u2.m_type;
	    });
}

void Material::addUniform(const MaterialUniform& uniform)
{
	uniforms_.push_back(uniform);
}



void Material::copyUniformsAndValuesFromMaterial(Material* m)
{
	uniforms_ = m->uniforms_;
	floatUniforms_ = m->floatUniforms_;
	vec2Uniforms_ = m->vec2Uniforms_;
	vec3Uniforms_ = m->vec3Uniforms_;
	vec4Uniforms_ = m->vec4Uniforms_;
	sampler2DUniforms_ = m->sampler2DUniforms_;
}

void Material::setUniformsAndSaveSetValues(const QVector<MaterialUniform>& uniforms)
{
}

void Material::setFloatUniform(const MaterialUniform& uniform, float value)
{
}

void Material::setVec2Uniform(const MaterialUniform& uniform, const vec2& value)
{
}

void Material::setVec3Uniform(const MaterialUniform& uniform, const vec3& value)
{
}

void Material::setVec4Uniform(const MaterialUniform& uniform, const vec4& value)
{
}

void Material::setSampler2DUniform(const MaterialUniform& uniform, Texture2D* texture)
{
}

float Material::floatUniformValue(const MaterialUniform& uniform) const
{
	return DEFAULT_FLOAT_VALUE;
}

vec2 Material::vec2UniformValue(const MaterialUniform& uniform) const
{
	return DEFAULT_VEC2_VALUE;
}

vec3 Material::vec3UniformValue(const MaterialUniform& uniform) const
{
	return DEFAULT_VEC3_VALUE;
}

vec4 Material::vec4UniformValue(const MaterialUniform& uniform) const
{
	return DEFAULT_VEC4_VALUE;
}

Texture2D* Material::sampler2DUniformValue(const MaterialUniform& uniform) const
{
	// This means that no texture has been assigned to this uniform yet!
	return DEFAULT_SAMPLER2D_VALUE;
}

const QMap<QString, float>& Material::floatUniforms() const
{
	return floatUniforms_;
}

const QMap<QString, vec2>& Material::vec2Uniforms() const
{
	return vec2Uniforms_;
}

const QMap<QString, vec3>& Material::vec3Uniforms() const
{
	return vec3Uniforms_;
}

const QMap<QString, vec4>& Material::vec4Uniforms() const
{
	return vec4Uniforms_;
}

const QMap<QString, Texture2D*>& Material::sampler2DUniforms() const
{
	return sampler2DUniforms_;
}

bool Material::internal() const
{
	return id() > 0;
}

QString Material::toString() const
{
	return QString::number(id_) + QString(") ") + name_;
}