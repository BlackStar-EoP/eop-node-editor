#pragma once

#include <QGraphicsItem>

class NodeConnection;
class NodePortConnectorWidget;

class NodeConnectionGraphicsItem : public QObject, public QGraphicsItem
{
Q_OBJECT
Q_INTERFACES(QGraphicsItem)
public:

	NodeConnectionGraphicsItem();

    bool add_port(NodePortConnectorWidget* port);
	void update_line(const QPointF& end_pos);

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
	QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private slots:
	void self_destruct();
	void update_port_positions();

public:
	void set_connection(NodeConnection* connection);
	NodeConnection* connection();

private:
    void update_bounding_rect();
    QPainterPath create_path() const;
    void set_start_pos(QPointF start_pos);
    void set_end_pos(QPointF end_pos);

private:
	NodeConnection* m_connection = nullptr;
	NodePortConnectorWidget* m_input_port = nullptr;
	NodePortConnectorWidget* m_output_port = nullptr;
	bool m_hover = false;

	QPointF m_input_pos;
	QPointF m_output_pos;
    std::optional<QPointF> m_line_pos;
    QRectF m_bounding_rect;
};