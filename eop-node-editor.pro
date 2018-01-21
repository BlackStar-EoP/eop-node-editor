QT       += core gui widgets

INCLUDEPATH += $$PWD/src

TARGET = eop-node-editor
TEMPLATE = app

SOURCES += $$PWD/src/app/main.cpp \
           $$PWD/src/app/mainwindow.cpp \
           $$PWD/src/controllers/NodeGraphController.cpp \
           $$PWD/src/model/Node.cpp \
           $$PWD/src/model/NodeConnection.cpp \
           $$PWD/src/model/NodePort.cpp \
		       $$PWD/src/model/NodeGraph.cpp \
           $$PWD/src/view/NodeConnectionGraphicsItem.cpp \
		       $$PWD/src/view/NodeGraphicsItem.cpp \
		       $$PWD/src/view/NodeGraphWidget.cpp \
		       $$PWD/src/view/NodeGraphScene.cpp \
           $$PWD/src/view/NodePortGraphicsItem.cpp

HEADERS += $$PWD/src/app/mainwindow.h \
           $$PWD/src/controllers/NodeGraphController.h \
           $$PWD/src/model/Node.h \
           $$PWD/src/model/NodeConnection.h \
           $$PWD/src/model/NodePort.h \
		       $$PWD/src/model/NodeGraph.h \
           $$PWD/src/view/NodeConnectionGraphicsItem.h \
           $$PWD/src/view/NodeGraphicsItem.h \
           $$PWD/src/view/NodeGraphWidget.h \
           $$PWD/src/view/NodeGraphScene.h \
           $$PWD/src/view/NodePortGraphicsItem.h