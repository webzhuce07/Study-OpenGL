#include <QApplication>
#include <iostream>
#include "myQGLWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!QGLFormat::hasOpenGL()) {
        std::cerr << "This system has no OpenGL support" << std::endl;
        return 1;
    }

    MyQGLWidget widget;
    widget.setWindowTitle(QObject::tr("3D Viewing Transform"));
	widget.resize(600, 600);
	widget.show();

    return app.exec();
}
