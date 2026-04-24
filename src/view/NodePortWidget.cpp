#include "NodePortWidget.h"

#include <QBoxLayout>
#include <QLabel>
#include <qwidget.h>

#include "model/NodePortModel.h"

#include "NodePortConnectorWidget.h"

NodePortWidget::NodePortWidget(NodePortModel* port_model, uint32_t port_index)
: m_port_model(port_model)
, m_port_index(port_index)
{
    init_ui();
}

void NodePortWidget::init_ui()
{
    QBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(2, 1, 2, 1);
    layout->setSpacing(4);

    if (m_port_model->port_type() == NodePortModel::INPUT)
    {
        layout->addWidget(m_port_model->connector_widget());
        if (QWidget* widget = m_port_model->widget(); widget != nullptr)
        {
            layout->addWidget(widget);
        }
    }
    else
    {
        layout->addStretch();
    }

    layout->addWidget(new QLabel(m_port_model->name()));

    if (m_port_model->port_type() == NodePortModel::OUTPUT)
    {
        if (QWidget* widget = m_port_model->widget(); widget != nullptr)
        {
            layout->addWidget(widget);
        }
        layout->addWidget(m_port_model->connector_widget());
    }
    else
    {
        layout->addStretch();
    }
}