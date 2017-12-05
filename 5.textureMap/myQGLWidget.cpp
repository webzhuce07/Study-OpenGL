#include <QtGui>
#include <QtOpenGL>
#include <glut.h>
#include "myQGLWidget.h"

MyQGLWidget::MyQGLWidget(QWidget *parent)
    : QGLWidget(parent),rotateAngle_(0.0f)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    QTimer *timer = new QTimer(this);                //创建一个定时器
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));  //将定时器的计时信号与updateGL()绑定
    timer->start(10);                                //以10ms为一个计时周期

}

void MyQGLWidget::initializeGL()
{
    loadGLTextures();
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //设置清除屏幕所用的颜色
    glShadeModel(GL_SMOOTH);   // 平滑方式，这也是默认方式
    glClearDepth(1.0);//设置深度缓存
    glEnable(GL_DEPTH_TEST);  // 启用深度测试
    glDepthFunc(GL_LEQUAL);  //设置深度测试类型
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//进行透视校正
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
    gluLookAt(0, 0, 12, 0, 0, 0, 0, 1, 0);
    drawCube();
}

void MyQGLWidget::loadGLTextures()
{
    QImage tex, buf;
    if(!buf.load(":/new/img/NeHe.jpg")){
        QImage dummy(128, 128, QImage::Format_RGB32);
        dummy.fill(Qt::green);
        buf = dummy;
    }
    tex = convertToGLFormat(buf);
    glGenTextures(1, &texture[0]);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

}

void MyQGLWidget::drawCube()
{
    glRotatef(rotateAngle_, 1.0f, 0.0f, 0.0f);
    static const GLfloat P1[3] = { 1.0, 1.0, 1.0 };
    static const GLfloat P2[3] = { -1.0, 1.0, 1.0 };
    static const GLfloat P3[3] = { -1.0, -1.0, 1.0 };
    static const GLfloat P4[3] = { 1.0, -1.0, 1.0 };
    static const GLfloat P5[3] = { 1.0, 1.0, -1.0 };
    static const GLfloat P6[3] = { -1.0, 1.0, -1.0 };
    static const GLfloat P7[3] = { -1.0, -1.0, -1.0 };
    static const GLfloat P8[3] = { 1.0, -1.0, -1.0 };

    static const GLfloat * const coords[6][4] = {
        { P1, P2, P3 ,P4}, { P6, P5, P8, P7 }, { P2, P6, P7, P3 }, { P5, P1, P4, P8 },{P5, P6, P2, P1},{P4, P3, P7, P8}
    };

    static const GLfloat T1[2] = { 0.0f, 0.0f };
    static const GLfloat T2[2] = { 1.0f, 0.0f };
    static const GLfloat T3[2] = { 1.0f, 1.0f };
    static const GLfloat T4[2] = { 0.0f, 1.0f };

    static const GLfloat * const tCoords[4] = {T1, T2, T3, T4};

    for (int i = 0; i < 6; ++i) {
        glBegin(GL_QUADS);
        for (int j = 0; j < 4; ++j) {
            glTexCoord2f(tCoords[j][0], tCoords[j][1]);
            glVertex3f(coords[i][j][0], coords[i][j][1],
                       coords[i][j][2]);
        }
        glEnd();
    }
    rotateAngle_ += 0.5f;  
}







