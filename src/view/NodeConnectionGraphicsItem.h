#pragma once

#include <QGraphicsLineItem>

#include "NodePortGraphicsItem.h"

class NodeConnection;

class NodeConnectionGraphicsItem : public QObject, public QGraphicsLineItem, public PortPositionListener
{
Q_OBJECT

	const QPointF OFFSET = QPointF(10, 10);

public:

	NodeConnectionGraphicsItem();
	~NodeConnectionGraphicsItem();

	void set_first_port(NodePortGraphicsItem* port);
	void update_line(const QPointF& end_pos);
	void set_second_port(NodePortGraphicsItem* port);

	void portPositionChanged() override;

	void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private slots:
	void self_destruct();

public:
	void set_connection(NodeConnection* connection);
	NodeConnection* connection();

private:
	NodeConnection* m_connection = nullptr;
	NodePortGraphicsItem* m_first_port = nullptr;
	NodePortGraphicsItem* m_second_port = nullptr;
	bool m_hover = false;
};
