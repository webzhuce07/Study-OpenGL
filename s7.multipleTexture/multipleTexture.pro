TEMPLATE      = app
QT            += core gui
QT            += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG       += console
HEADERS       = myQGLWidget.h
SOURCES       = main.cpp \
                myQGLWidget.cpp

RESOURCES += \
    image.qrc
