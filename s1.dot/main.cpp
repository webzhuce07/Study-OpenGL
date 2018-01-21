#include <QApplication>
#include <iostream>
#include "myQGlWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!QGLFormat::hasOpenGL()) {
        std::cerr << "This system has no OpenGL support" << std::endl;
        return 1;
    }

    MyQGLWidget widget;
    widget.setWindowTitle(QObject::tr("OpenGLWindow"));
    widget.resize(300, 300);
    widget.show();

    return app.exec();
}
