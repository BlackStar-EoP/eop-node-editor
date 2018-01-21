#pragma once

class NodePort;

class NodeConnection
{
public:
	NodeConnection(const NodePort& input, const NodePort& output);
	~NodeConnection() = default;

private:
	const NodePort& m_input;
	const NodePort& m_output;
};