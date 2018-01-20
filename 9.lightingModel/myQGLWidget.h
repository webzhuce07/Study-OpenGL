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

private:
    void loadGLTextures();
    void drawCube();
    GLfloat rotateAngle_;
    GLuint texture[1];
    bool lightFlag_;  //flag of opening or closing light
};

#endif
