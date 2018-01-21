#ifndef XIAOGLWIDGET_H
#define XIAOGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Compatibility>

class xiaoGLWidget : public QOpenGLWidget,protected QOpenGLFunctions_4_3_Compatibility
{
public:
    xiaoGLWidget(QWidget *parent);
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w,int h);
public:
    void installShader();
    void sendDataToOpenGL();
};

#endif // XIAOGLWIDGET_H
