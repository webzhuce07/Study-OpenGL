TEMPLATE      = app
QT           += opengl
LIBS         += -lopengl32
CONFIG       += console

INCLUDEPATH += ../glut
LIBS        += -L../glut\
-lglut\
-lglut32


HEADERS       = myQGLWidget.h
SOURCES       = main.cpp \
                myQGLWidget.cpp

RESOURCES += \
    src.qrc




