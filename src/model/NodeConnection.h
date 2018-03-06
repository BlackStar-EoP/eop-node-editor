#pragma once

class NodePortModel;

class NodeConnection
{
public:
	enum EDirection
	{
		INCOMING,
		OUTGOING
	};

	NodeConnection(NodePortModel& input, NodePortModel& output);
	~NodeConnection();

	const NodePortModel& input() const;
	const NodePortModel& output() const;

private:
	NodePortModel& m_input;
	NodePortModel& m_output;
};