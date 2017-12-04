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
//    glShadeModel(GL_FLAT);      // 单色方式
    glShadeModel(GL_SMOOTH);   // 平滑方式，这也是默认方式
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
        glColor3f(1.0f, 0.0f, 0.0f);                    //设置当前色为红色
        glVertex3f(0.0f, 1.0f, -6.0f);
        glColor3f(0.0f, 1.0f, 0.0f);                    //设置当前色为绿色
        glVertex3f(-1.0f,-1.0f, -6.0f);
        glColor3f(0.0f, 0.0f, 1.0f);                    //设置当前色为蓝色
        glVertex3f(1.0f,-1.0f, -6.0f);
    glEnd();
}

void MyQGLWidget::drawPoints()
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(10.0); //设置点的大小
    glBegin(GL_POINTS);
        glVertex3f(0.0f, 1.0f, -6.0f);
        glVertex3f(-1.0f,-1.0f, -6.0f);
        glVertex3f(1.0f,-1.0f, -6.0f);
    glEnd();
}
