#include <StdHeader.h>

InitWindows::InitWindows(QObject *parent)
    : QObject(parent)
{
    mousePressed = false;
    qApp->installEventFilter(this);
}

bool InitWindows::eventFilter(QObject *obj, QEvent *evt)
{
    QWidget *wdt = (QWidget *)obj;
    if (!wdt->property("ablemove").toBool()) {
        return QObject::eventFilter(obj, evt);
    }

    QMouseEvent *event = static_cast<QMouseEvent *>(evt);
    if (event->type() == QEvent::MouseButtonPress) {
        if (event->button() == Qt::LeftButton) {
            mousePressed = true;
            mousePoint = event->globalPos() - wdt->pos();
            return true;
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        mousePressed = false;
        return true;
    } else if (event->type() == QEvent::MouseMove) {
        if (mousePressed && (event->buttons() && Qt::LeftButton)) {
            wdt->move(event->globalPos() - mousePoint);
            return true;
        }
    }

    return QObject::eventFilter(obj, evt);
}
