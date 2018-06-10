#include "NodeGraphView.h"

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

NodeGraphView::NodeGraphView(QGraphicsScene* scene)
: QGraphicsView(scene)
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setMaxSize();
	centerOn(0, 0);

	zoomDelta = 0.1;
	panSpeed = 1;
	_doMousePanning = false;
	_doKeyZoom = false;
	_scale = 1.0;
}

qreal NodeGraphView::getScale() const
{
	return _scale;
}

void NodeGraphView::mouseMoveEvent(QMouseEvent * event)
{
	if (_doMousePanning) {
		QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(_lastMousePos);
		pan(mouseDelta);
	}

	QGraphicsView::mouseMoveEvent(event);
	_lastMousePos = event->pos();
}

void NodeGraphView::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::RightButton) {
		_lastMousePos = event->pos();
		_doMousePanning = true;
	}

	QGraphicsView::mousePressEvent(event);
}

void NodeGraphView::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == Qt::RightButton) {
		_doMousePanning = false;
	}

	QGraphicsView::mouseReleaseEvent(event);
}

void NodeGraphView::wheelEvent(QWheelEvent *event)
{
	QPoint scrollAmount = event->angleDelta();

	// Apply zoom.
	scrollAmount.y() > 0 ? zoomIn() : zoomOut();
}

void NodeGraphView::setMaxSize()
{
	setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);
}

void NodeGraphView::zoom(float scaleFactor)
{
	scale(scaleFactor, scaleFactor);
	_scale *= scaleFactor;
}

void NodeGraphView::zoomIn()
{
	zoom(1 + zoomDelta);
}

void NodeGraphView::zoomOut()
{
	zoom(1 - zoomDelta);
}

void NodeGraphView::pan(QPointF delta)
{
	// Scale the pan amount by the current zoom.
	delta *= _scale;
	delta *= panSpeed;

	// Have panning be anchored from the mouse.
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	QPoint newCenter(VIEW_WIDTH / 2 - delta.x(), VIEW_HEIGHT / 2 - delta.y());
	centerOn(mapToScene(newCenter));

	// For zooming to anchor from the view center.
	setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}