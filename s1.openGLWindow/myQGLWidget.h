#ifndef MY_QGLWIDGET_H
#define MY_QGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class MyQGLWidget : public QOpenGLWidget , protected QOpenGLFunctions
{
    Q_OBJECT

public:
	MyQGLWidget(QWidget *parent = 0);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

};

#endif
