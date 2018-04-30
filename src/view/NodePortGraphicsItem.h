#pragma once

#include <QObject>
#include <QGraphicsItem>

#include <stdint.h>
#include <set>

class NodePortModel;

// I don't like this, but 5.9.2 doesn't have signalsfor qgraphics item, the model doesn't know anything about positions of the ports, 
// so the only sensible way to make the line respond is to make it listen to the PortGraphicsItem.. briliant design..
class PortPositionListener
{
public:
	virtual void portPositionChanged() = 0;
};

class NodePortGraphicsItem :public QObject, public QGraphicsItem
{
	Q_OBJECT
public:
	NodePortGraphicsItem(QGraphicsItem* parent, NodePortModel* port_model, uint32_t port_index);
	~NodePortGraphicsItem();

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

	void select();
	NodePortModel* port_model() const;

	void add_port_position_listener(PortPositionListener* listener);
	void remove_port_position_listener(PortPositionListener* listener);

private:
	void notify_position_listeners();

private slots:
	void selfdestruct();

private:
	QRectF m_bounding_rect = QRect(0, 0, 20, 20);
	NodePortModel* m_port_model;
	uint32_t m_port_index = 0;
	uint32_t m_selected = false;

	// TODO ? NodeGraphicsItem* m_parent = nullptr;
	std::set<PortPositionListener*> m_port_position_listeners;
};