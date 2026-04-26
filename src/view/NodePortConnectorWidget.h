#pragma once

#include <QWidget>

class NodePortModel;

class NodePortConnectorWidget : public QWidget
{
    Q_OBJECT
public:
    NodePortConnectorWidget(NodePortModel* port_model, uint32_t port_index);
    virtual ~NodePortConnectorWidget();

    void paintEvent(QPaintEvent* event) override;
    QSize sizeHint() const override;

    NodePortModel* port_model() const;
    void select();

    void update_pos();

    /**
     * Get the position of the attachment point, where the connection line attaches to the connector
     * in scene coordinates. This assumes this widget is used inside a QGraphicsProxyWidget.
     */
    QPointF attachment_scene_pos() const;

signals:
    void position_changed();

protected:
    void moveEvent(QMoveEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QPoint center_point() const;

private:
    NodePortModel* m_port_model = nullptr;
    uint32_t m_port_index = 0;
    bool m_selected = false;

    static constexpr float PORT_RADIUS = 10.0f;
};