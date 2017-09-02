#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include <QGLWidget>

class EmptyWindow : public QGLWidget
{
    Q_OBJECT

public:
    EmptyWindow(QWidget *parent = 0);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

private:

};

#endif
