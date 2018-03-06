#pragma once

#include <src/model/NodeModel.h>
#include <src/model/NodePortModel.h>

class Material;
class NodePortModel;

class FloatPortModel : public NodePortModel
{
public:
	FloatPortModel(const QString& name)	: m_name(name) {}
	EPortType port_type() const override { return NodePortModel::INPUT; }
	QString port_label() const override	{ return m_name; }
	QString name() override { return "float"; }
	bool may_connect_to(const NodePortModel& port_model) const override { return true; }


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
	bool may_connect_to(const NodePortModel& port_model) const override { return true; }

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
	bool may_connect_to(const NodePortModel& port_model) const override { return true; }

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
	bool may_connect_to(const NodePortModel& port_model) const override { return true; }

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
	bool may_connect_to(const NodePortModel& port_model) const override { return true; }

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
	bool may_connect_to(const NodePortModel& port_model) const override { return true; }

private:
	QString m_name;
};

class MaterialNodeModel : public NodeModel
{
public:
	MaterialNodeModel(Material* material);

	void create_input_port_models() override;
	void create_output_port_models() override;
	QString title() const override;
	void on_connection(NodeConnection::EDirection direction, NodeModel* target_model, NodePortModel* target_port_model) override;

	Material* material() const;
private:
	Material* m_material = nullptr;

};