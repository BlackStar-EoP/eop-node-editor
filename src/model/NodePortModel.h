#pragma once

#include <stdint.h>
#include <QObject>
#include <QString>
#include <QVector>
#include <QPointer>

#include "NodeConnection.h"

class NodeModel;
class NodePortConnectorWidget;

class NodePortModel : public QObject
{
	Q_OBJECT
public:
	enum EPortType : int32_t
	{
        NONE = 0,
        INPUT = 1,
        OUTPUT = 2,
        BOTH = INPUT | OUTPUT
	};

    NodePortModel(NodeModel* owning_node);
	virtual ~NodePortModel();
	virtual EPortType port_type() const = 0;
	virtual QString port_label() const = 0;
	virtual QString name() const = 0;
	virtual bool may_connect_to(const NodePortModel& port_model) const = 0;
	virtual bool supports_multiple_connections() const = 0;
	virtual QString type() const = 0;
    virtual QWidget* create_widget() { return nullptr; }

    virtual QJsonObject user_data() const;
    virtual bool load_from_user_data(const QJsonObject& user_data);

signals:
    void node_port_connections_changed();
	void node_port_model_destroyed();
    void connection_updated(const NodePortModel* port_model, const NodeConnection* connection);

public:
    NodePortConnectorWidget* connector_widget();
    void update_connector_position();

    NodeModel* owning_node() const;

    void set_exposing_node(NodeModel* node_model);
    NodeModel* exposing_node() const;

    void set_enabled(bool enabled);
    bool enabled() const;

	NodeConnection* connection(uint32_t index) const;
	uint32_t num_connections() const;
	void add_connection(NodeConnection* connection);
	void remove_connection(NodeConnection* connection);
	bool has_connection(NodePortModel* other_port_model);
    const QVector<NodeConnection*> connections() const;
    bool accepts_new_connections() const;

    template <class NodeType>
    NodeType* get_connected_node_by_type()
    {
        assert(!supports_multiple_connections());
        if (m_connections.isEmpty())
        {
            return nullptr;
        }
        return qobject_cast<NodeType*>(m_connections.front()->other(this)->owning_node());
    }

    /**
     * Send a notification to all connected nodes.
     */
    void notify_node_updated() const;

private slots:
    void on_connection_updated(const NodeConnection* connection);

private:
    /**
     * Node this port directly connects to. That is also the node responsible for clean up.
     * Follow this node when traversing the full graph.
     */
	NodeModel* m_owning_node_model = nullptr;
    /**
     * Node that exposes this port in the editor. Can be the same as the owning node, but can also be another node that
     * exposes a subgraph.
     * Use this node for UI activities and saving the graph shown to the user.
     */
    NodeModel* m_exposing_node_model = nullptr;

    bool m_enabled = true;

	QVector<NodeConnection*> m_connections;
    QPointer<NodePortConnectorWidget> m_connector_widget = nullptr;
};