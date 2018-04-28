#pragma once

#define CUSTOM_LINE_ITEM
#ifdef CUSTOM_LINE_ITEM

#include <QGraphicsLineItem>

#include "NodePortGraphicsItem.h"

class NodeConnection;

class NodeConnectionGraphicsItem : public QObject, public QGraphicsItem, public PortPositionListener
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

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;
	void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
	QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

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

	QPointF m_start_pos;
	QPointF m_end_pos;
};

#else

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

#endif