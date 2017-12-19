#pragma once

#include "controllers/NodeGraphController.h"

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
	NodeGraphController m_controller;
};