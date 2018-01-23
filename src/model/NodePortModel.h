#pragma once

#include <stdint.h>
#include <QString>

class NodePortModel
{
public:
	enum EPortType : int32_t
	{
		INPUT,
		OUTPUT
	};

	virtual ~NodePortModel() = default;
	virtual EPortType port_type() const = 0;
	virtual QString port_label() const = 0;
};