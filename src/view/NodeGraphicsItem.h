#pragma once

#include <QObject>
#include <model/NodeModel.h>
#include <QGraphicsItem>
#include <QVector>

class NodePortGraphicsItem;

class NodeGraphicsItem : public QObject, public QGraphicsItem, public INodeModelListener
{
	Q_OBJECT
public:
	NodeGraphicsItem(NodeModel* node_model);

	NodePortGraphicsItem* find_input_port(NodePortModel* input_port);
	NodePortGraphicsItem* find_output_port(NodePortModel* output_port);

	QRectF boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
	void initUI();
	void init_input_ports();
	void init_output_ports();

	void recalculate_size();
	QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
	void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

	void node_model_changed() override;
	void output_nodes_changed() override;

private slots:
	void self_destruct();

public:
	NodeModel* node_model();

private:
	QRectF m_bounding_rect = QRect(0, 0, 100, 50);
	NodeModel* m_node_model = nullptr;
	QVector<NodePortGraphicsItem*> m_input_ports;
	QVector<NodePortGraphicsItem*> m_output_ports;

	uint32_t m_port_start_y = 0;
	bool m_hover = false;
};