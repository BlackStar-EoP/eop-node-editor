#pragma once

#include <controllers/NodeGraphController.h>
#include <model/NodeGraph.h>

#include <QWidget>
#include <QVector>
#include <QJsonObject>

class QGraphicsView;
class NodeGraphScene;
class NodeFactory;
class NodeModel;

class NodeGraphWidget : public QWidget
{
	Q_OBJECT
public:
	NodeGraphWidget(QWidget* parent = nullptr);
	~NodeGraphWidget();

	void give_node_factory(NodeFactory* factory);

public:
	const QVector<NodeModel*>& nodes() const;
	void set_persisted();
	bool is_persisted() const;

	void new_graph();
	QJsonObject save_graph() const;
	void load_graph(const QJsonObject& json_data);

signals:
	void node_graph_changed();
	void message(const QString& message, bool status);

private:
	bool parse_node(const QJsonObject& node, QMap<uint32_t, NodeModel*>& node_models);

private:
	QGraphicsView* m_view = nullptr;
	NodeGraphScene* m_scene = nullptr;
	NodeGraphController m_controller;
	NodeGraph m_node_graph;
	NodeFactory* m_node_factory = nullptr;
};