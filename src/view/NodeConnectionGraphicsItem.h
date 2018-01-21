#pragma once

#include <QGraphicsLineItem>

#include "NodePortGraphicsItem.h"


class NodeConnectionGraphicsItem : public QGraphicsLineItem, public PortPositionListener
{
	const QPointF OFFSET = QPointF(10, 10);

public:

	NodeConnectionGraphicsItem() = default;
	~NodeConnectionGraphicsItem();

	void set_first_port(NodePortGraphicsItem* port);
	void update_line(const QPointF& end_pos);
	void set_second_port(NodePortGraphicsItem* port);

	void portPositionChanged() override;

private:
	NodePortGraphicsItem* m_first_port = nullptr;
	NodePortGraphicsItem* m_second_port = nullptr;
};
