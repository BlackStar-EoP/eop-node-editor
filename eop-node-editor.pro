QT       += core gui widgets

TARGET = eop-node-editor
TEMPLATE = app


SOURCES += $$pwd/src/app/main.cpp \
           $$pwd/src/model/Node.cpp

HEADERS += $$pwd/src/model/Node.h \
           $$pwd/src/view/NodeGraphicsItem.h
