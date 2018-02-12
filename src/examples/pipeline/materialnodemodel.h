#pragma once

#include <src/model/NodeModel.h>
#include <src/model/NodePortModel.h>

class Material;
class NodePortModel;

class FloatPortModel : public NodePortModel
{
public:
	FloatPortModel(const QString& name)	: m_name(name)	{}
	EPortType port_type() const override { return NodePortModel::INPUT; }
	QString port_label() const override	{ return m_name; }
	QString name() override { return "float"; }


private:
	QString m_name;
};

class Vec2PortModel : public NodePortModel
{
public: 
	Vec2PortModel(const QString& name) : m_name(name) {}
	EPortType port_type() const override { return NodePortModel::INPUT; }
	QString port_label() const override { return m_name; }
	QString name() override { return "vec2"; }

private:
	QString m_name;
};
class Vec3PortModel : public NodePortModel
{
public:
	Vec3PortModel(const QString& name) : m_name(name) {}
	EPortType port_type() const override { return NodePortModel::INPUT; }
	QString port_label() const override { return m_name; }
	QString name() override { return "vec3"; }

private:
	QString m_name;
};
class Vec4PortModel : public NodePortModel
{
public:
	Vec4PortModel(const QString& name) : m_name(name) {}
	EPortType port_type() const override { return NodePortModel::INPUT; }
	QString port_label() const override { return m_name; }
	QString name() override { return "vec3"; }

private:
	QString m_name;
};

class TexturePortModel : public NodePortModel
{
public:
	TexturePortModel(const QString& name) : m_name(name) {}
	EPortType port_type() const override { return NodePortModel::INPUT; }
	QString port_label() const override { return m_name; }
	QString name() override { return "texture"; }

private:
	QString m_name;
};

class ShaderOutputPortModel : public NodePortModel
{
public:
	ShaderOutputPortModel(const QString& name) : m_name(name) {}
	EPortType port_type() const override { return NodePortModel::OUTPUT; }
	QString port_label() const override { return m_name; }
	QString name() override { return "shaderoutput"; }

private:
	QString m_name;
};

class MaterialNodeModel : public NodeModel
{
public:
	MaterialNodeModel(Material* material);

	void create_port_models() override;
	void destroy_port_models() override;
	QString title() const override;
	uint32_t num_ports() const override;
	NodePortModel* port_model(uint32_t port_nr) override;

private:
	Material* m_material = nullptr;
	QVector<NodePortModel*> m_port_models;

};