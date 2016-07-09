#ifndef INITWINDOWS_H
#define INITWINDOWS_H
#include <QObject>
#include <QPoint>

class InitWindows: public QObject
{
    Q_OBJECT
public:
    explicit InitWindows(QObject *parent = 0);

private:
    QPoint mousePoint;
    bool mousePressed;

protected:
    bool eventFilter(QObject *obj, QEvent *evt);

signals:

public slots:
};
#endif // INITWINDOWS_H
