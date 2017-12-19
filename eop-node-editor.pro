QT       += core gui widgets

TARGET = eop-node-editor
TEMPLATE = app

SOURCES += $$PWD/src/app/main.cpp \
           $$PWD/src/app/mainwindow.cpp \
           $$PWD/src/model/Node.cpp \
           $$PWD/src/model/NodeInput.cpp \
		       $$PWD/src/model/NodeOutput.cpp \
		       $$PWD/src/model/NodeGraph.cpp \
		       $$PWD/src/view/NodeGraphicsItem.cpp \
		       $$PWD/src/view/NodeGraphWidget.cpp \
		       $$PWD/src/view/NodeGraphScene.cpp \
           $$PWD/src/view/NodePortGraphicsItem.cpp

HEADERS += $$PWD/src/app/mainwindow.h \
           $$PWD/src/model/Node.h \
           $$PWD/src/model/NodeInput.h \
		       $$PWD/src/model/NodeOutput.h \
		       $$PWD/src/model/NodeGraph.h \
           $$PWD/src/view/NodeGraphicsItem.h \
           $$PWD/src/view/NodeGraphWidget.h \
           $$PWD/src/view/NodeGraphScene.h \
           $$PWD/src/view/NodePortGraphicsItem.h