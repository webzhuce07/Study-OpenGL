TEMPLATE      = app
QT           += opengl
LIBS         += -lopengl32
CONFIG       += console
HEADERS       = myQGLWidget.h
SOURCES       = main.cpp \
                myQGLWidget.cpp

RESOURCES += \
    src.qrc
