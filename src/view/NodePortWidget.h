#pragma once

#include <QString>
#include <QWidget>

class NodePortModel;

class NodePortWidget : public QWidget
{
    Q_OBJECT
public:
    NodePortWidget(NodePortModel* port_model, const QString& label);

private:
    void init_ui();

private:
    NodePortModel* m_port_model = nullptr;
    QString m_label;
};