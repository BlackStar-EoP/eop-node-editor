#include "NodeGraphController.h"

#include "model/Node.h"
#include "model/NodeGraph.h"

#include <QPoint>
#include <assert.h>

NodeGraphController::NodeGraphController(NodeGraph& node_graph)
: m_node_graph(node_graph)
{
}

void NodeGraphController::set_first_connection_port(NodePort* port)
{
	m_first_connection_port = port;
}

void NodeGraphController::set_second_connection_port(NodePort* port)
{
	m_second_connection_port = port;
}

const NodeConnection* NodeGraphController::create_connection()
{
	if (m_first_connection_port == nullptr || m_second_connection_port == nullptr)
		return nullptr;

	// TODO, input ports can only have 1 connection, but output ports can have multiple
	if (m_first_connection_port->connection() != nullptr || m_second_connection_port->connection() != nullptr)
		return nullptr;

	if (m_first_connection_port == m_second_connection_port)
		return nullptr;

	if (m_first_connection_port->port_type() == m_second_connection_port->port_type())
		return nullptr;

	if (m_first_connection_port->node() == m_second_connection_port->node())
		return nullptr;

	NodePort* input_port = nullptr;
	NodePort* output_port = nullptr;

	if (m_first_connection_port->port_type() == NodePortModel::INPUT)
	{
		input_port = m_first_connection_port;
		output_port = m_second_connection_port;
	}
	else if (m_second_connection_port->port_type() == NodePortModel::INPUT)
	{
		input_port = m_second_connection_port;
		output_port = m_first_connection_port;
	}

	// If output goes to an input of node earlier in graph, we have a circular dependency
	if (m_node_graph.scan_left(output_port->node(), input_port->node()))
		return nullptr;

	// If input comes from node later in graph, we also have a circular dependency

	m_first_connection_port = nullptr;
	m_second_connection_port = nullptr;

	NodeConnection* connection = new NodeConnection(*input_port, *output_port);
	m_node_graph.give_connection(connection);
	return connection;
}


#include <examples/pipeline/materialnodemodel.h>
#include <examples/pipeline/material.h>

Node* NodeGraphController::add_node(const QPointF& position)
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
		material->addUniform(MaterialUniform(MaterialUniform::VEC2, "Vec3 var"));
	for (uint32_t i = 0; i < num_vec4; ++i)
		material->addUniform(MaterialUniform(MaterialUniform::VEC3, "Vec4 var"));
	for (uint32_t i = 0; i < num_tex; ++i)
		material->addUniform(MaterialUniform(MaterialUniform::VEC4, "Float var"));

	uint32_t num_outputs = rand() % 2 + 1;
	for (uint32_t i = 0; i < num_outputs; ++i)
		material->addOutput(ShaderOutput(i, MaterialUniform::VEC4, "fragcolor"));


	MaterialNodeModel* model = new MaterialNodeModel(material);
	model->create_port_models();
	Node* n = new Node(position, model);
	m_node_graph.give_node(n);
	return n;
}