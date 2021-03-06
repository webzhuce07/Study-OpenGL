#include <QtGui>
#include <QtOpenGL>
#include <glut.h>
#include "myQGLWidget.h"

MyQGLWidget::MyQGLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
}

void MyQGLWidget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //设置清除屏幕所用的颜色
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);  // 启用深度测试
    glEnable(GL_CULL_FACE);   // 所作深度测试的类型
}

void MyQGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);               //选择投影矩阵
    glLoadIdentity();                          //重置投影矩阵
    GLfloat x = GLfloat(width) / height;
    glFrustum(-x, +x, -1.0, +1.0, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);                // 选择模型观察矩阵
}

void MyQGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawTriangles();
}

void MyQGLWidget::drawTriangles()
{
    glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 1.0f, -6.0f);
        glVertex3f(-1.0f,-1.0f, -6.0f);
        glVertex3f(1.0f,-1.0f, -6.0f);
    glEnd();
}

void MyQGLWidget::drawPoints()
{
    glBegin(GL_POINTS);
        glVertex3f(0.0f, 1.0f, -6.0f);
        glVertex3f(-1.0f,-1.0f, -6.0f);
        glVertex3f(1.0f,-1.0f, -6.0f);
    glEnd();
}
