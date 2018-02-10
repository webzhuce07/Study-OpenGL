#include <QtGui>
#include "myQGLWidget.h"

MyQGLWidget::MyQGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      NumVertices(6)
{
}

void MyQGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    initShader();
}

void MyQGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void MyQGLWidget::paintGL()
{
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void MyQGLWidget::initShader()
{
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);
    GLfloat vertices[6][2] = {
        {-0.90, -0.90},
        {0.85, -0.90},
        {-0.90, 0.85},
        {0.90, -0.85},
        {0.90, 0.90},
        {-0.85, 0.90}
    };

    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    const char*  vertexShaderCode =
        "#version 430 \n"
        ""
        "layout(location=0) in vec4 vPosition;"
        ""
        "void main()"
        "{"
        "   gl_Position = vPosition;"
        "}";

    const char* fragmentShaderCode =
        "#version 430 \r \n"
        ""
        "out vec4 fColor;"
        ""
        "void main()"
        "{"
        "   fColor = vec4(0.0, 0.0, 1.0, 1.0);"
        "}";
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    const char* adapter[1];
    adapter[0] = vertexShaderCode;
    glShaderSource(vertexShaderID, 1, adapter, 0);

    adapter[0] = fragmentShaderCode;
    glShaderSource(fragmentShaderID, 1, adapter, 0);

    glCompileShader(vertexShaderID);
    glCompileShader(fragmentShaderID);

    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);

    glLinkProgram(programID);
    glUseProgram(programID);

}
