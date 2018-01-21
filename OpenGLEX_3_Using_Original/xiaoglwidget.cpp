#include "xiaoglwidget.h"

extern const char* vertexShaderCode;
extern const char* fragmentShaderCode;


xiaoGLWidget::xiaoGLWidget(QWidget *parent)
{

}

void xiaoGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    sendDataToOpenGL();
    installShader();
}

void xiaoGLWidget::paintGL()
{
    initializeOpenGLFunctions();
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,NULL);
}

void xiaoGLWidget::resizeGL(int w, int h)
{

}

void xiaoGLWidget::sendDataToOpenGL()
{
    initializeOpenGLFunctions();
    const GLfloat RED_TRIANGLE_DEPTH = -0.5f;
    const GLfloat BLUE_TRIANGLE_DEPTH = 0.5f;
    //顶点和颜色数据
    GLfloat verts[]={
        +0.0f, -1.0f, RED_TRIANGLE_DEPTH,
        +1.0f,+0.0f,+0.0f,
        +1.0f, +1.0f, RED_TRIANGLE_DEPTH,
        +1.0f,+0.0f,+0.0f,
        -1.0f, +1.0f, RED_TRIANGLE_DEPTH,
        +1.0f,+0.0f,+0.0f,

        +0.0f, +1.0f, BLUE_TRIANGLE_DEPTH,
        +0.0f, +0.0f, +1.0f,
        -1.0f, -1.0f, BLUE_TRIANGLE_DEPTH,
        +0.0f, +0.0f, +1.0f,
        +1.0f, -1.0f, BLUE_TRIANGLE_DEPTH,
        +0.0f, +0.0f, +1.0f,
    };

    GLuint vertsBufferID;
    glGenBuffers(1,&vertsBufferID);
    glBindBuffer(GL_ARRAY_BUFFER,vertsBufferID);
    glBufferData(GL_ARRAY_BUFFER,sizeof(verts),verts,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6,(char*)(sizeof(float) * 3));

    GLushort indices[]={0,1,2,3,4,5};
    GLuint  indexBufferID;
    glGenBuffers(1,&indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
}

void xiaoGLWidget::installShader()
{
    initializeOpenGLFunctions();
    
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    const char* adapter[1];
    adapter[0] = vertexShaderCode;
    glShaderSource(vertexShaderID,1,adapter,0);

    adapter[0] = fragmentShaderCode;
    glShaderSource(fragmentShaderID,1,adapter,0);

    glCompileShader(vertexShaderID);
    glCompileShader(fragmentShaderID);

    GLuint programID = glCreateProgram();

    glAttachShader(programID,vertexShaderID);
    glAttachShader(programID,fragmentShaderID);

    glLinkProgram(programID);
    glUseProgram(programID);
}
