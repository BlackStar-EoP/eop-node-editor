#include "NodePortConnectorWidget.h"

#include <QPainter>
#include <QGraphicsProxyWidget>
#include <qgraphicsproxywidget.h>
#include <qobject.h>
#include <qwidget.h>

#include "model/NodePortModel.h"

#include "EditorColorScheme.h"

NodePortConnectorWidget::NodePortConnectorWidget(NodePortModel* port_model, uint32_t port_index)
    : m_port_model(port_model)
    , m_port_index(port_index)
{
}

NodePortConnectorWidget::~NodePortConnectorWidget()
{
}

void NodePortConnectorWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    const QPoint center = center_point();
    const int x = center.x() - PORT_RADIUS;
    const int y = center.y() - PORT_RADIUS;

	painter.setPen(EditorColorScheme::gridMajorColor());
	if (m_port_model->num_connections() > 0)
	{
		painter.setBrush(EditorColorScheme::connection_color());
		painter.drawEllipse(x, y, 2*PORT_RADIUS, 2*PORT_RADIUS);
	}
	else
	{
		painter.drawEllipse(x, y, 2*PORT_RADIUS, 2*PORT_RADIUS);
	}
}

QSize NodePortConnectorWidget::sizeHint() const
{
    return QSize(2 * PORT_RADIUS + 2, 2 * PORT_RADIUS + 2);
}

NodePortModel* NodePortConnectorWidget::port_model() const
{
    return m_port_model;
}

QPointF NodePortConnectorWidget::attachment_scene_pos() const
{
    const QWidget* current = parentWidget();
    while (current->parentWidget() != nullptr)
    {
        current = current->parentWidget();
    }
    QGraphicsProxyWidget* proxy = current->graphicsProxyWidget();
    if (!proxy) return QPointF();

    QPoint pos_in_proxy = mapTo(proxy->widget(), center_point());
    return proxy->mapToScene(QPointF(pos_in_proxy));
}

void NodePortConnectorWidget::select()
{
    // TODO
}

void NodePortConnectorWidget::update_pos()
{
    emit position_changed();
}

void NodePortConnectorWidget::moveEvent(QMoveEvent* event)
{
    QWidget::moveEvent(event);
    emit position_changed();
    qDebug() << "Moved: " << event;
}

void NodePortConnectorWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    emit position_changed();
    qDebug() << "Resized: " << event;
}

QPoint NodePortConnectorWidget::center_point() const
{
    return QPoint(size().width() / 2, size().height() / 2);
}