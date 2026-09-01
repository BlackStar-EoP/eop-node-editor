#pragma once

#include <QObject>

class NodePortModel;

class NodeConnection : public QObject
{
	Q_OBJECT
public:
	enum EDirection
	{
		INCOMING,
		OUTGOING
	};

	NodeConnection(NodePortModel* input, NodePortModel* output);
	~NodeConnection();

	bool composed_by(NodePortModel* first_port, NodePortModel* second_port) const;
	NodePortModel* input() const;
	NodePortModel* output() const;
	NodePortModel* other(NodePortModel* port) const;

	void set_valid(bool valid);
	bool is_valid() const;

    void notify_updated(const NodePortModel* source);

signals:
	void connection_destroyed();
    void input_updated(const NodeConnection* connection);
    void output_updated(const NodeConnection* connection);

private:
	NodePortModel* m_input = nullptr;
	NodePortModel* m_output = nullptr;
	bool m_valid = true;
};