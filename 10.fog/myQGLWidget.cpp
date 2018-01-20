#include <QtGui>
#include <QtOpenGL>
#include <glut.h>
#include "myQGLWidget.h"

MyQGLWidget::MyQGLWidget(QWidget *parent)
    : QGLWidget(parent),lightFlag_(false)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    rotateAngleX_ = -21.0;
    rotateAngleY_ = -57.0;
    rotateAngleZ_ = 0.0;
    fogFlag_ = false;
}

void MyQGLWidget::initializeGL()
{
    loadGLTextures();
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //设置清除屏幕所用的颜色
    glShadeModel(GL_SMOOTH);   // 平滑方式，这也是默认方式
    glClearDepth(1.0);//设置深度缓存
    glEnable(GL_DEPTH_TEST);  // 启用深度测试
    glDepthFunc(GL_LEQUAL);  //设置深度测试类型
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//进行透视校正

    GLfloat lightAmbient[] = {1.0f, 0.5f, 0.5f, 1.0f};  //环境光参数
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  //漫散光参数
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f}; //镜面反射参数
    GLfloat lightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f}; //光源位置,局部光源

    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);     //设置环境光
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);     //设置漫射光
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);   //镜面反射后
    glLightfv(GL_LIGHT1, GL_POSITION,lightPosition);   //设置光源位置
    glEnable(GL_LIGHT1);                                //启动一号光源


   //雾部分
    GLfloat fogColor[] = {0.5f, 0.5f, 0.5f, 1.0f};      //雾的颜色
    //GL_EXP  GL_LINEAR GL_EXP2
    glFogi(GL_FOG_MODE, GL_LINEAR);                        //设置雾气的初始模式
    glFogfv(GL_FOG_COLOR, fogColor);                    //设置雾的颜色
    glFogf(GL_FOG_DENSITY, 0.35);                       //设置雾的密度
    glHint(GL_FOG_HINT, GL_DONT_CARE);                  //设置系统如何计算雾气
    glFogf(GL_FOG_START, 12.0f);                         //雾的开始位置
    glFogf(GL_FOG_END, 60.0f);                           //雾的结束位置
    glEnable(GL_FOG);                                   //启动雾效果


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

void MyQGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void MyQGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();

    if (event->buttons() & Qt::LeftButton) {
        rotateAngleX_ += 180 * dy;
        rotateAngleY_ += 180 * dx;
        updateGL();
    } else if (event->buttons() & Qt::RightButton) {
        rotateAngleX_ += 180 * dy;
        rotateAngleZ_ += 180 * dx;
        updateGL();
    }
    lastPos = event->pos();
}

void MyQGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_L:
        lightFlag_ = !lightFlag_;
        if (lightFlag_){
            glEnable(GL_LIGHTING);
        }
        else{
            glDisable(GL_LIGHTING);
        }
        break;
    case Qt::Key_F:
        fogFlag_ = !fogFlag_;
        if (fogFlag_){
            glEnable(GL_FOG);
        }
        else{
            glDisable(GL_FOG);
        }
        break;
    }
    updateGL();
}

void MyQGLWidget::loadGLTextures()
{
    QImage tex, buf;
    if(!buf.load(":/new/img/Crate.bmp")){
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
    glRotatef(rotateAngleX_, 1.0, 0.0, 0.0);
    glRotatef(rotateAngleY_, 0.0, 1.0, 0.0);
    glRotatef(rotateAngleZ_, 0.0, 0.0, 1.0);

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

    static const GLfloat Normal[6][3] = { {0.0, 0.0, 1.0},
                                        {0.0, 0.0, -1.0},
                                        {-1.0, 0.0, 0.0},
                                        {1.0, 1.0, 0.0},
                                        {0.0, 1.0, 0.0},
                                        {0.0, -1.0, 0.0}};

    for (int i = 0; i < 6; ++i) {
        glBegin(GL_QUADS);
        glNormal3f(Normal[i][0], Normal[i][1], Normal[i][2]);
        for (int j = 0; j < 4; ++j) {
            glTexCoord2f(tCoords[j][0], tCoords[j][1]);
            glVertex3f(coords[i][j][0], coords[i][j][1],
                       coords[i][j][2]);
        }
        glEnd();
    }
}







