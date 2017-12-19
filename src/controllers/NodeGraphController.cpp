#include "NodeGraphController.h"

#include "model/Node.h"

#include <QPoint>

Node* NodeGraphController::add_node(const QPointF& position)
{
	Node* n = new Node(position);
	int inputs = rand() % 10;
	int outputs = rand() % 10;

	for (int i = 0; i < inputs; ++i)
		n->add_input("Input1");
	for (int i = 0; i < outputs; ++i)
		n->add_output("Output1");
	return n;
}