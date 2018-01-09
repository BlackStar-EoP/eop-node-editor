#pragma once

#include <QGraphicsItem>

#include <stdint.h>

class NodePort;

class NodePortGraphicsItem : public QGraphicsItem
{
public:
	NodePortGraphicsItem(QGraphicsItem* parent, const NodePort& node_port, uint32_t port_index);

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	void select();

private:
	void set_port_position();

private:
	QRectF m_bounding_rect = QRect(0, 0, 20, 20);
	const NodePort& m_node_port;
	uint32_t m_port_index = 0;
	uint32_t m_selected = false;
};