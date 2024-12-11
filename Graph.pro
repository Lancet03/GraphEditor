QT       += core gui
QT += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    EdgeLine.cpp \
    Graph.cpp \
    GraphEdge.cpp \
    GraphPlane.cpp \
    GraphVertex.cpp \
    VertexCircle.cpp \
    adjacencymatrix.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    EdgeLine.h \
    Graph.h \
    GraphEdge.h \
    GraphPlane.h \
    GraphVertex.h \
    Mode.h \
    VertexCircle.h \
    adjacencymatrix.h \
    mainwindow.h

FORMS += \
    adjacencymatrix.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets.qrc
