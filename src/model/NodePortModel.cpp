#include "NodePortModel.h"

#include "NodeModel.h"
#include "NodeConnection.h"

#include "view/NodePortConnectorWidget.h"

#include <assert.h>

NodePortModel::NodePortModel(NodeModel* owning_node)
    : m_owning_node_model(owning_node)
    , m_exposing_node_model(owning_node)
{
}

NodePortModel::~NodePortModel()
{
    assert(m_connections.isEmpty());
	emit node_port_model_destroyed();
}

NodePortConnectorWidget* NodePortModel::connector_widget()
{
    if (m_connector_widget.isNull())
    {
        m_connector_widget = new NodePortConnectorWidget(this, 0u);
        m_connector_widget->set_enabled(m_enabled);
    }
    return m_connector_widget;
}

void NodePortModel::update_connector_position()
{
    if (m_connector_widget != nullptr)
        m_connector_widget->update_pos();
}

NodeModel* NodePortModel::owning_node() const
{
	return m_owning_node_model;
}

void NodePortModel::set_exposing_node(NodeModel* node_model)
{
	m_exposing_node_model = node_model;
}

NodeModel* NodePortModel::exposing_node() const
{
	return m_exposing_node_model;
}

void NodePortModel::set_enabled(bool enabled)
{
    m_enabled = enabled;
    if (m_connector_widget != nullptr)
    {
        m_connector_widget->set_enabled(enabled);
    }
}

bool NodePortModel::enabled() const
{
    return m_enabled;
}

NodeConnection* NodePortModel::connection(uint32_t index) const
{
	assert(static_cast<int32_t>(index) < m_connections.size());
	return m_connections[index];
}

void NodePortModel::add_connection(NodeConnection* connection)
{
	assert(m_connections.indexOf(connection) == -1);
	m_connections.push_back(connection);

    assert(m_owning_node_model != nullptr);
	m_owning_node_model->connection_added(this, connection);
}

void NodePortModel::remove_connection(NodeConnection* connection)
{
    int32_t index = m_connections.indexOf(connection);
    assert(index != -1);
    m_connections.remove(index);
    assert(m_owning_node_model != nullptr);
    m_owning_node_model->connection_removed(this, connection);
}

uint32_t NodePortModel::num_connections() const
{
	return static_cast<uint32_t>(m_connections.size());
}

bool NodePortModel::has_connection(NodePortModel* other_port_model)
{
	for (NodeConnection* connection : m_connections)
	{
		if (connection->composed_by(this, other_port_model))
			return true;
	}

	return false;
}

const QVector<NodeConnection*> NodePortModel::connections() const
{
    return m_connections;
}

bool NodePortModel::accepts_new_connections() const
{
    return (supports_multiple_connections() || m_connections.isEmpty()) && m_enabled;
}