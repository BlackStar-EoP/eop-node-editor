QT       += core gui widgets

TARGET = eop-node-editor
TEMPLATE = app

SOURCES += $$PWD/src/app/main.cpp \
           $$PWD/src/app/mainwindow.cpp \
           $$PWD/src/model/Node.cpp

HEADERS += $$PWD/src/app/mainwindow.h \
           $$PWD/src/model/Node.h \
           $$PWD/src/view/NodeGraphicsItem.h
