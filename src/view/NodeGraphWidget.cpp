#include "NodeGraphWidget.h"

#include "NodeGraphScene.h"

#include <QBoxLayout>
#include <QGraphicsView>

NodeGraphWidget::NodeGraphWidget(QWidget* parent)
: QWidget(parent)
{
	m_scene = new NodeGraphScene(this);
	QBoxLayout* layout = new QBoxLayout(QBoxLayout::Down);
	m_view = new QGraphicsView(m_scene);
	layout->addWidget(m_view);

	setLayout(layout);
}