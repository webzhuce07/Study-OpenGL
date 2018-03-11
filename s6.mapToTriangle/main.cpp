#include <QApplication>
#include "myQGlWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MyQGLWidget widget;
    widget.setWindowTitle(QObject::tr("Texture To Triangle"));
    widget.resize(300, 300);
    widget.show();

    return app.exec();
}
