# Study-OpenGL
In Qt 5.6.x, Study OpenGL

无边框窗口
=====
　　在实际软件开发中，UI设计师给你的界面显然Qt的本地化样式是无法满足的。比如QQ的登陆界面，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20180501122336542?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlYnpodWNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
　　要是实现这么炫的界面，我们首先要将MainFrame设置FramelessWindowHint无边框，然后再自定义界面。但是设置FramelessWindowHint后，带来主要问题是无法移动窗口以及无法使用鼠标拖拽缩放窗口大小。本文就是总结如何解决这两个问题，不涉及界面的美化。

解决方法
====

实现移动窗口功能
--------
　　重新定义mousePressEvent和mouseMoveEvent来实现移动窗口。一般软件比如Visual Studio，鼠标在标题栏上移动窗口。所以根据你的需求，在你需要的QWidget重新定义这两个函数。在示例代码里只为了说明方法，直接在QMainFrame里实现。在主窗口的任何地方都可以移动窗口。

```
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        last_mouse_position_ = event->globalPos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (!event->buttons().testFlag(Qt::LeftButton))
            return;
    const QPoint position = pos() + event->globalPos() - last_mouse_position_; //the position of mainfrmae + (current_mouse_position - last_mouse_position)
    move(position.x(), position.y());
    last_mouse_position_ = event->globalPos();
}
```
**运行结果:**
![这里写图片描述](https://img-blog.csdn.net/20180501122503313?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlYnpodWNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

实现缩放大小功能
--------
　　使用nativeEvent进行窗体缩放。主要代码如下：

```
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    int m_nBorder = 5;
    Q_UNUSED(eventType)
    MSG *param = static_cast<MSG *>(message);

    switch (param->message)
    {
        case WM_NCHITTEST:
        {
            int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
            int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

            //// if mouse in the child Qwidget of mainfrme, we should not handle it
            //if (childAt(nX, nY) != NULL)
            //	return QWidget::nativeEvent(eventType, message, result);

            *result = HTCAPTION;

            // if mouse in the border of mainframe,we zoom mainframe
            if ((nX > 0) && (nX < m_nBorder))
                *result = HTLEFT;

            if ((nX > this->width() - m_nBorder) && (nX < this->width()))
                *result = HTRIGHT;

            if ((nY > 0) && (nY < m_nBorder))
                *result = HTTOP;

            if ((nY > this->height() - m_nBorder) && (nY < this->height()))
                *result = HTBOTTOM;

            if ((nX > 0) && (nX < m_nBorder) && (nY > 0)
                && (nY < m_nBorder))
                *result = HTTOPLEFT;

            if ((nX > this->width() - m_nBorder) && (nX < this->width())
                && (nY > 0) && (nY < m_nBorder))
                *result = HTTOPRIGHT;

            if ((nX > 0) && (nX < m_nBorder)
                && (nY > this->height() - m_nBorder) && (nY < this->height()))
                *result = HTBOTTOMLEFT;

            if ((nX > this->width() - m_nBorder) && (nX < this->width())
                && (nY > this->height() - m_nBorder) && (nY < this->height()))
                *result = HTBOTTOMRIGHT;

            if (*result == HTCAPTION)
            {
                return false;
            }
            return true;
        }
    }
    return QMainWindow::nativeEvent(eventType, message, result);
}
```
**运行结果：**         
![这里写图片描述](https://img-blog.csdn.net/20180501122556528?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlYnpodWNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

[项目代码下载](https://github.com/webzhuce07/Qt-Programming/tree/master/FramelessWindow)




