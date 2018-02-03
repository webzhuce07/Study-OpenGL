#ifndef MY_QGLWIDGET_H
#define MY_QGLWIDGET_H

#include <QGLWidget>

class MyQGLWidget : public QGLWidget
{
    Q_OBJECT

public:
	MyQGLWidget(QWidget *parent = 0);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent* event); //handle key events
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void timerEvent(QTimerEvent* event);

private:
    void loadGLTextures();
    void draw();
    GLfloat rotateAngleX_;
    GLfloat rotateAngleY_;
    GLfloat rotateAngleZ_;
    QPoint lastPos;
    GLuint texture[1];
    bool lightFlag_;  //flag of opening or closing light
    bool fogFlag_;
    float points[45][45][3];    // The Array For The Points On The Grid Of Our "Wave"
    int wiggle_count = 0;		// Counter Used To Control How Fast Flag Waves

};

#endif
