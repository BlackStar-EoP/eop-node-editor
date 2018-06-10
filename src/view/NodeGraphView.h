#pragma once

#include <QGraphicsView>
#include <QWheelEvent>
#include <QKeyEvent>

/*
Original code by Luis Lpz on gist.github.com

https://gist.github.com/luis-l

*/

class NodeGraphView : public QGraphicsView
{
public:
	NodeGraphView(QGraphicsScene* scene);

	qreal panSpeed;
	qreal zoomDelta;

	void pan(QPointF delta);
	void zoom(float scaleFactor);
	void zoomIn();
	void zoomOut();

	qreal getScale() const;

protected:
	void mouseMoveEvent(QMouseEvent*) override;
	void mousePressEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	void wheelEvent(QWheelEvent*) override;

private:

	// Flags to determine if zooming or panning should be done.
	bool _doMousePanning;
	bool _doKeyZoom;

	QPoint _lastMousePos;
	qreal _scale;

	void setMaxSize();
};