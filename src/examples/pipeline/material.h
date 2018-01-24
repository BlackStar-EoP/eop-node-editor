#pragma once

#include <stdint.h>

#include <QString>

class MaterialUniform
{
public:
	enum EUniformType : int32_t
	{
		FLOAT,
		VEC2,
		VEC3,
		VEC4,
		MAT3,
		MAT4,
		INT,
		SAMPLER_1D,
		SAMPLER_2D,
		SAMPLER_3D,
		SAMPLER_CUBE,
		UNSUPPORTED_UNIFORM
	};

	EUniformType m_type = FLOAT;
	QString m_name = "";
	MaterialUniform(EUniformType type, const QString& name)
	: m_type(type)
	, m_name(name)
	{
	}
	
	MaterialUniform() = default;

};

class ShaderOutput
{
public:
	ShaderOutput(int32_t location, MaterialUniform::EUniformType type, const QString& name)
	: m_location(location)
	, m_type(type)
	, m_name(name)
	{
	}

	ShaderOutput() = default;

	QString toString() const
	{
		return QString::number(m_location) + QString(" - vec4 ") + name();
	}

public:
	int32_t location() const { return m_location; }
	MaterialUniform::EUniformType type() { return m_type; }
	const QString& name() const { return m_name; }

private:
	int32_t m_location = -1;
	MaterialUniform::EUniformType m_type = MaterialUniform::UNSUPPORTED_UNIFORM;
	QString m_name;
};


class vec2
{
public:
	vec2(float) {}
};

class vec3
{
public:
	vec3(float) {}
};
class vec4
{
public:
	vec4(float) {}
};

#include <QMap>
#include <QString>
#include <QVector>

class Shader;
class DeviseMaterialParameters;
class Texture2D;

class Material
{
public:
	Material(uint32_t id, const QString& name);
	uint32_t id() const;
	const QString& name() const;
	
	void setShaders(Shader* vertex, Shader* fragment, Shader* geom = nullptr, Shader* tessControl = nullptr, Shader* tessEval = nullptr);
    void setVertexShader(Shader* vertexShader);
    void setFragmentShader(Shader* fragmentShader);
    void setGeomShader(Shader* geomShader);
    void setTessControlShader(Shader* tessControlShader);
    void setTessEvalShader(Shader* tessEvalShader);

	DeviseMaterialParameters* createParameters();
	void setParameters(DeviseMaterialParameters& parameters);

	const QVector<ShaderOutput>& outputs() const;
	void addOutput(const ShaderOutput& output);

	const QVector<MaterialUniform>& uniforms() const;
	void getUniformsSortedByTypeAndName(QVector<MaterialUniform>& uniforms);
    void addUniform(const MaterialUniform& uniform);
	
	void setUniformsAndSaveSetValues(const QVector<MaterialUniform>& uniforms);
	void copyUniformsAndValuesFromMaterial(Material* m);

	void setFloatUniform(const MaterialUniform& uniform, float value);
	void setVec2Uniform(const MaterialUniform& uniform, const vec2& value);
	void setVec3Uniform(const MaterialUniform& uniform, const vec3& value);
	void setVec4Uniform(const MaterialUniform& uniform, const vec4& value);
	void setSampler2DUniform(const MaterialUniform& uniform, Texture2D* texture);

	float floatUniformValue(const MaterialUniform& uniform) const;
	vec2 vec2UniformValue(const MaterialUniform& uniform) const;
	vec3 vec3UniformValue(const MaterialUniform& uniform) const;
	vec4 vec4UniformValue(const MaterialUniform& uniform) const;
	Texture2D* sampler2DUniformValue(const MaterialUniform& uniform) const;

	const QMap<QString, float>& floatUniforms() const;
	const QMap<QString, vec2>& vec2Uniforms() const;
	const QMap<QString, vec3>& vec3Uniforms() const;
	const QMap<QString, vec4>& vec4Uniforms() const;
	const QMap<QString, Texture2D*>& sampler2DUniforms() const;

	bool internal() const;

	QString toString() const;

private:


private:
	uint32_t id_ = 0;
	QString name_ = "";

public:
	Shader* vertexShader_ = nullptr;
	Shader* fragmentShader_ = nullptr;
	Shader* geometryShader_ = nullptr;
	Shader* tessControlShader_ = nullptr;
	Shader* tessEvalShader_ = nullptr;
	QVector<ShaderOutput> outputs_;
	QVector<MaterialUniform> uniforms_;

	// This is not the neatest solution, but one that works =)
	QMap<QString, float> floatUniforms_;
	QMap<QString, vec2> vec2Uniforms_;
	QMap<QString, vec3> vec3Uniforms_;
	QMap<QString, vec4> vec4Uniforms_;
	QMap<QString, Texture2D*> sampler2DUniforms_;

	const float DEFAULT_FLOAT_VALUE = 0.0f;
	const vec2 DEFAULT_VEC2_VALUE = vec2(0);
	const vec3 DEFAULT_VEC3_VALUE = vec3(0);
	const vec4 DEFAULT_VEC4_VALUE = vec4(0);
	Texture2D* DEFAULT_SAMPLER2D_VALUE = nullptr;
	/* These sampler types still need to be implemented!
		MAT3,
		MAT4,
		INT,
		SAMPLER_1D,
		SAMPLER_3D,
		SAMPLER_CUBE,
	*/
};