#pragma once

#include <src/model/NodeModel.h>
#include <src/model/NodePortModel.h>

class RenderTarget;
class Material;

class RenderTargetInputPortModel : public NodePortModel
{
public:
	RenderTargetInputPortModel(const QString& name) : m_name(name) {}
	EPortType port_type() const override { return NodePortModel::INPUT; }
	QString port_label() const override { return m_name; }
	QString name() override { return "rendertargetinput"; }
	bool may_connect_to(const NodePortModel& port_model) const override { return true; }

private:
	QString m_name;
};

class RenderTargetTexturePortModel : public NodePortModel
{
public:
	RenderTargetTexturePortModel(const QString& name) : m_name(name) {}
	EPortType port_type() const override { return NodePortModel::OUTPUT; }
	QString port_label() const override { return m_name; }
	QString name() override { return "rendertargettexture"; }
	bool may_connect_to(const NodePortModel& port_model) const override { return true; }

private:
	QString m_name;
};



class RenderTargetNodeModel : public NodeModel
{
public:
	RenderTargetNodeModel(RenderTarget* rendertarget);

	void create_input_port_models() override;
	void create_output_port_models() override;
	QString title() const override;
	void on_connection(NodeModel* target_model, NodePortModel* target_port_model) override;

private:
	RenderTarget* m_target = nullptr;
	Material* m_material = nullptr;
};