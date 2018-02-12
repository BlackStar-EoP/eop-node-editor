#include "mainwindow.h"

#include "src/view/NodeGraphWidget.h"

#include <examples/pipeline/pipelinenodefactory.h>

MainWindow::MainWindow()
{
	NodeGraphWidget* w = new NodeGraphWidget(this);
	w->give_node_factory(new PipeLineNodeFactory());
	setCentralWidget(w);
	w->show();
}