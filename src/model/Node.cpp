#include "Node.h"

Node::Node()
{

}

Node::~Node()
{

}
void Node::add_input(const NodeInput& input)
{
	m_input_ports.push_back(input);
}

void Node::add_output(const NodeOutput& output)
{
	m_output_ports.push_back(output);
}
