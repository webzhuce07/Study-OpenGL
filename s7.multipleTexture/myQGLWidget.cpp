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
    loadGLTextures();
    initShader();
}

void MyQGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void MyQGLWidget::loadGLTextures()
{
    QImage tex, buf;
    if(!buf.load(":/new/img/texture.jpg")){
        QImage dummy(128, 128, QImage::Format_RGB32);
        dummy.fill(Qt::green);
        buf = dummy;
    }
    tex = QGLWidget::convertToGLFormat(buf);
    glGenTextures(1, &texture[0]);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    if(!buf.load(":/new/img/lena.jpg")){
        QImage dummy(128, 128, QImage::Format_RGB32);
        dummy.fill(Qt::green);
        buf = dummy;
    }
    tex = QGLWidget::convertToGLFormat(buf);
    glGenTextures(1, &texture[1]);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
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
        // 位置                 // 纹理坐标
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.5f, 1.0f,   // 顶部
    };

    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));

    const char*  vertexShaderCode =
        "#version 430 \n"
        ""
        "layout(location = 0) in vec3 vPosition;"
        "layout(location = 1) in vec2 textureCoord;"
        "out vec2 myTextureCoord;"
        ""
        "void main()"
        "{"
        "   gl_Position = vec4(vPosition, 1.0);"
        "   myTextureCoord = textureCoord;"
        "}";

    const char* fragmentShaderCode =
        "#version 430 \r \n"
        ""
        "in vec2 myTextureCoord;"
        "out vec4 fColor;"
        "uniform sampler2D tex1;"
        "uniform sampler2D tex2;"
        ""
        "void main()"
        "{"
        "   fColor = texture(tex1, myTextureCoord) + texture(tex2, myTextureCoord);"
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
    if (!compiled)
    {
        std::cout<<"vertexShaderID";
        GLsizei len;
        glGetShaderiv( vertexShaderID, GL_INFO_LOG_LENGTH, &len );

        GLchar* log = new GLchar[len+1];
        glGetShaderInfoLog( vertexShaderID, len, &len, log );
        std::cerr << "Shader compilation failed: " << log << std::endl;
        delete [] log;
    }
    glCompileShader(fragmentShaderID);

    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compiled );
    if (!compiled)
    {
        std::cout<<"fragmentShaderID";
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
    if (!linked)
        std::cout<<"Error";
    glUseProgram(programID);

    GLint texLoc1 = glGetUniformLocation(programID, "tex1");
    glUniform1i(texLoc1, 0); //GL_TEXTURE0


    GLint texLoc2 = glGetUniformLocation(programID, "tex2");
    glUniform1i(texLoc2, 1); //GL_TEXTURE1

}
