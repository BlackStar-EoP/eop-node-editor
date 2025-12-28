cmake_minimum_required(VERSION 4.0)

project(eop-node-editor CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
qt_standard_project_setup()

qt_add_library(${PROJECT_NAME}-lib STATIC
  src/controllers/NodeGraphController.cpp
  src/controllers/NodeGraphController.h
  src/model/NodeConnection.cpp
  src/model/NodeConnection.h
  src/model/NodeFactory.cpp
  src/model/NodeFactory.h
  src/model/NodeGraph.cpp
  src/model/NodeGraph.h
  src/model/NodeModel.cpp
  src/model/NodeModel.h
  src/model/NodePortModel.cpp
  src/model/NodePortModel.h
  src/model/NodeType.cpp
  src/model/NodeType.h
  src/view/DefaultColorSchemeProvider.h
  src/view/EditorColorScheme.cpp
  src/view/EditorColorScheme.h
  src/view/IColorSchemeProvider.h
  src/view/NodeConnectionGraphicsItem.cpp
  src/view/NodeConnectionGraphicsItem.h
  src/view/NodeGraphicsItem.cpp
  src/view/NodeGraphicsItem.h
  src/view/NodeGraphScene.cpp
  src/view/NodeGraphScene.h
  src/view/NodeGraphView.cpp
  src/view/NodeGraphView.h
  src/view/NodeGraphWidget.cpp
  src/view/NodeGraphWidget.h
  src/view/NodePortGraphicsItem.cpp
  src/view/NodePortGraphicsItem.h
)

target_include_directories(${PROJECT_NAME}-lib
  PUBLIC
    src
)

target_link_libraries(${PROJECT_NAME}-lib
  PUBLIC
    Qt6::Core
    Qt6::Widgets
)

qt_add_library(${PROJECT_NAME}-examples STATIC EXCLUDE_FROM_ALL
  src/examples/pipeline/material.cpp
  src/examples/pipeline/material.h
  src/examples/pipeline/materialnodemodel.cpp
  src/examples/pipeline/materialnodemodel.h
  src/examples/pipeline/pipelinenodefactory.cpp
  src/examples/pipeline/pipelinenodefactory.h
  src/examples/pipeline/rendertarget.cpp
  src/examples/pipeline/rendertarget.h
  src/examples/pipeline/rendertargetmodel.cpp
  src/examples/pipeline/rendertargetmodel.h
)

target_include_directories(${PROJECT_NAME}-examples
  PUBLIC
    src
)

target_link_libraries(${PROJECT_NAME}-examples
  PUBLIC
    ${PROJECT_NAME}-lib
)

qt_add_executable(${PROJECT_NAME} EXCLUDE_FROM_ALL
  src/app/main.cpp
  src/app/mainwindow.cpp
  src/app/mainwindow.h
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    .
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    ${PROJECT_NAME}-examples
    ${PROJECT_NAME}-lib
)
