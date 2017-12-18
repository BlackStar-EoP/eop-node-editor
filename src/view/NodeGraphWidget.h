#pragma once

#include <QWidget>

class QGraphicsView;
class NodeGraphScene;

class NodeGraphWidget : public QWidget
{
public:
	NodeGraphWidget(QWidget* parent = nullptr);

private:
	QGraphicsView* m_view = nullptr;
	NodeGraphScene* m_scene = nullptr;
};