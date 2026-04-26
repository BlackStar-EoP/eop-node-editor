#pragma once

#include <QWidget>

class NodePortModel;

class NodePortWidget : public QWidget
{
    Q_OBJECT
public:
    NodePortWidget(NodePortModel* port_model, uint32_t port_index);

private:
    void init_ui();

private:
    NodePortModel* m_port_model = nullptr;
    // TODO: Do we still need this?
    uint32_t m_port_index = 0;
};