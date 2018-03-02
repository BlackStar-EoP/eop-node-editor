#include "NodePortModel.h"

NodePortModel::~NodePortModel()
{
	emit node_port_model_destroyed();
}