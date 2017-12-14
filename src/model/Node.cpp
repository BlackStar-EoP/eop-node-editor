#pragma once

#include <QVector>

class Node
{
public:
	Node();
	~Node();

private:
	QVector m_input_ports;
	QVector m_output_ports;
}