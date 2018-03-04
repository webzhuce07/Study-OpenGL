#include <QtGui>
#include "myQGLWidget.h"
#include <iostream>

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
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //glBindVertexArray(VAOs[Triangles]);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);
    //glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void MyQGLWidget::initShader()
{
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);
    // create a cube
    //    v6----- v5
    //   /|      /|
    //  v1------v0|
    //  | |     | |
    //  | |v7---|-|v4
    //  |/      |/
    //  v2------v3
    GLfloat vertices[] = {
        // position                 // color
        0.5,  0.5,  0.5,     1.0,  1.0,  1.0,  // v0 White
        -0.5,  0.5,  0.5,     1.0,  0.0,  1.0,  // v1 Magenta
        -0.5, -0.5,  0.5,     1.0,  0.0,  0.0,  // v2 Red
        0.5, -0.5,  0.5,     1.0,  1.0,  0.0,  // v3 Yellow
        0.5, -0.5, -0.5,     0.0,  1.0,  0.0,  // v4 Green
        0.5,  0.5, -0.5,     0.0,  1.0,  1.0,  // v5 Cyan
        -0.5,  0.5, -0.5,     0.0,  0.0,  1.0,  // v6 Blue
        -0.5, -0.5, -0.5,     0.0,  0.0,  0.0   // v7 Black
    };

    //顶点索引
    GLushort indices[36] = {
        0, 1, 2,   0, 2, 3,    // 前

        0, 3, 4,   0, 4, 5,    // 右

        0, 5, 6,   0, 6, 1,    // 上

        1, 6, 7,   1, 7, 2,    // 左

        7, 4, 3,   7, 3, 2,    // 下

        4, 7, 6,   4, 6, 5     // 后

     };


    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));

    GLuint  indexBufferID;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    const char*  vertexShaderCode =
        "#version 430 \n"
        ""
        "layout(location = 0) in vec3 vPosition;"
        "layout(location = 1) in vec3 color;"
        "out vec3 myColor;"
        ""
        "void main()"
        "{"
        "   mat4 rotatey = mat4(0.5, 0.0, 0.85, 0.0,   0.0,1.0,0.0,0.0, -0.85,0.0,0.5,0.0,  0.0,0.0, 0.0,1.0);"
        "   mat4 rotatex = mat4(1.0, 0.0, 0.0, 0.0,   0.0,0.7,-0.7,0.0, 0.0,0.7,0.7,0.0,  0.0,0.0, 0.0,1.0);"
        "   gl_Position =  rotatex * vec4(vPosition, 1.0);"
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

    GLint compiled;
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compiled );
    if ( !compiled ) { std::cout<<"vertexShaderID";
        GLsizei len;
        glGetShaderiv( vertexShaderID, GL_INFO_LOG_LENGTH, &len );

        GLchar* log = new GLchar[len+1];
        glGetShaderInfoLog( vertexShaderID, len, &len, log );
        std::cerr << "Shader compilation failed: " << log << std::endl;
        delete [] log;}

    glCompileShader(fragmentShaderID);

    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compiled );
    if ( !compiled ) { std::cout<<"fragmentShaderID";
        GLsizei len;
        glGetShaderiv( fragmentShaderID, GL_INFO_LOG_LENGTH, &len );

        GLchar* log = new GLchar[len+1];
        glGetShaderInfoLog( fragmentShaderID, len, &len, log );
        std::cerr << "Shader compilation failed: " << log << std::endl;
        delete [] log;
    }

    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);

    glLinkProgram(programID);
    GLint linked;
        glGetProgramiv( programID, GL_LINK_STATUS, &linked );
    if ( !linked ) { std::cout<<"Error";}
    glUseProgram(programID);

}
