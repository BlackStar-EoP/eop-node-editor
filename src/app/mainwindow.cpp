#include "mainwindow.h"

#include "src/view/NodeGraphWidget.h"

MainWindow::MainWindow()
{
	NodeGraphWidget* w = new NodeGraphWidget(this);
	setCentralWidget(w);
	w->show();
}