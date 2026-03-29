#include "NodeGraphWidget.h"

#include "NodeGraphScene.h"

#include <model/NodeFactory.h>
#include "model/NodeGraphLoader.h"
#include <model/NodeModel.h>
#include <model/NodePortModel.h>

#include "NodeGraphView.h"

#include <QMap>
#include <QSet>
#include <QBoxLayout>
#include <QGraphicsView>
#include <QJsonArray>

#include <assert.h>

NodeGraphWidget::NodeGraphWidget(QWidget* parent)
: QWidget(parent)
, m_controller(m_node_graph)
{
	m_scene = new NodeGraphScene(this, m_controller);
	QBoxLayout* layout = new QBoxLayout(QBoxLayout::Down);
	m_view = new NodeGraphView(m_scene);
	layout->addWidget(m_view);

	setLayout(layout);

	connect(&m_controller, SIGNAL(node_graph_changed()), this, SIGNAL(node_graph_changed()));
	connect(&m_controller, SIGNAL(message(const QString&, bool)), this, SIGNAL(message(const QString&, bool)));
}

NodeGraphWidget::~NodeGraphWidget()
{
	m_node_graph.clear();
	delete m_node_factory;
}

void NodeGraphWidget::give_node_factory(NodeFactory* factory)
{
	m_controller.set_node_factory(factory);
	m_node_factory = factory;
}

const QVector<NodeModel*>& NodeGraphWidget::nodes() const
{
	return m_node_graph.nodes();
}

void NodeGraphWidget::set_persisted()
{
	m_controller.set_persisted();
}

bool NodeGraphWidget::is_persisted() const
{
	return m_controller.is_persisted();
}

void NodeGraphWidget::new_graph()
{
    clear();
    update();
}

QJsonObject NodeGraphWidget::save_graph() const
{
    return NodeGraphLoader::save(m_node_graph);
}

void NodeGraphWidget::load_graph(const QJsonObject& json_data)
{
    NodeGraphLoader loader(m_node_graph, m_controller, *m_node_factory);
    if (!loader.load(json_data))
    {
        emit message(loader.last_error(), false);
    }
    set_persisted();
}

void NodeGraphWidget::adopt_graph(NodeGraph& source_graph)
{
    clear();

    // Transfer new graph and display
    QVector<NodeModel*> transferred = source_graph.release_nodes();
    for (NodeModel* node : transferred)
    {
        node->set_controller(&m_controller);
        m_node_graph.give_node(node);
    }

    const QVector<NodeModel*>& nodes = m_node_graph.nodes();
    for (NodeModel* node : nodes)
    {
        node->create_widget();
        node->sync_widget_from_model();
    }

    m_scene->build_scene_from_graph(nodes);

    set_persisted();

    m_view->update();
}

void NodeGraphWidget::clear()
{
	m_controller.clear_graph();
    for (NodeModel* node : m_node_graph.nodes())
    {
        node->set_widget(nullptr);
    }
    m_scene->clear_all();
    m_node_graph.clear();
}