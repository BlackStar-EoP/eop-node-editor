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

private:
	void set_port_position();

private:
	QRectF m_bounding_rect = QRect(0, 0, 10, 10);
	const NodePort& m_node_port;
	uint32_t m_port_index = 0;
};