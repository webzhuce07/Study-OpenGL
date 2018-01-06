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

private:
    void loadGLTextures();
    void drawCube();
    GLfloat rotateAngle_;
    GLuint textureID[6];
};

#endif
