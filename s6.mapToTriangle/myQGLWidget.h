#ifndef MY_QGLWIDGET_H
#define MY_QGLWIDGET_H

#include <QOpenGLWidget>
#include <QtOpenGL>
#include <QOpenGLFunctions_4_3_Compatibility>

class MyQGLWidget : public QOpenGLWidget , protected QOpenGLFunctions_4_3_Compatibility
{
    Q_OBJECT

public:
	MyQGLWidget(QWidget *parent = 0);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

private:
    enum VAO_IDs {Triangles, NumVAOs};
    enum Buffer_IDs {ArrayBuffer, NumBuffers};
    enum Attrib_IDs {vPosition = 0, vColor = 1};
    GLuint VAOs[NumVAOs];
    GLuint Buffers[NumBuffers];
    const GLuint NumVertices;
    void initShader();
    void loadGLTextures();
    GLuint texture[1];
};

#endif
