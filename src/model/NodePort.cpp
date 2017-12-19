#include "NodePort.h"

NodePort::NodePort(EPortType port_type, const QString& port_label)
: m_port_type(port_type)
, m_port_label(port_label)
{

}

NodePort::~NodePort()
{

}

NodePort::EPortType NodePort::port_type() const
{
	return m_port_type;
}

const QString& NodePort::port_label() const
{
	return m_port_label;
}
