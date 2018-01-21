#pragma once

class NodePort;

class NodeConnection
{
public:
	NodeConnection(NodePort& input, NodePort& output);
	~NodeConnection();

	const NodePort& input() const;
	const NodePort& output() const;

private:
	NodePort& m_input;
	NodePort& m_output;
};