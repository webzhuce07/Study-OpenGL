#include <QtGui>
#include <QtOpenGL>
#include "myQGLWidget.h"

MyQGLWidget::MyQGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

void MyQGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
}

void MyQGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void MyQGLWidget::paintGL()
{
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
}
