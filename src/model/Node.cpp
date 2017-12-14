#pragma once

#include <QVector>

class Node
{
public:
	Node();
	~Node();

private:
	QVector<int> m_input_ports;
	QVector<int> m_output_ports;
};