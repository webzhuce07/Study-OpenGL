#include <QApplication>
#include <iostream>
#include "emptyWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!QGLFormat::hasOpenGL()) {
        std::cerr << "This system has no OpenGL support" << std::endl;
        return 1;
    }

    EmptyWindow emptyWindow;
    emptyWindow.setWindowTitle(QObject::tr("EmptyWindow"));
    emptyWindow.resize(300, 300);
    emptyWindow.show();

    return app.exec();
}
