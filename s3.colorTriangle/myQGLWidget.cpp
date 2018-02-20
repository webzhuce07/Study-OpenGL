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
    GLfloat vertices[] = {
        // 位置                 // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));

    const char*  vertexShaderCode =
        "#version 430 \n"
        ""
        "layout(location = 0) in vec3 vPosition;"
        "layout(location = 1) in vec3 color;"
        "out vec3 myColor;"
        ""
        "void main()"
        "{"
        "   gl_Position = vec4(vPosition, 1.0);"
        "   myColor = color;"
        "}";

    const char* fragmentShaderCode =
        "#version 430 \r \n"
        ""
        "in vec3 myColor;"
        "out vec4 fColor;"
        ""
        "void main()"
        "{"
        "   fColor = vec4(myColor, 1.0f);"
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
