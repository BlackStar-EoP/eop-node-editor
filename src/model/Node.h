#pragma once

#include <QVector>

#include "NodeInput.h"
#include "NodeOutput.h"

class Node
{
public:
	Node();
	~Node();

public:
	void add_input(const NodeInput& input);
	void add_output(const NodeOutput& output);

private:
	QVector<NodeInput> m_input_ports;
	QVector<NodeOutput> m_output_ports;
};