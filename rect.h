#ifndef RECT_H
#define RECT_H
#include <QRect>
#include <QPoint>

class Rect : public QRect
{
public:
    Rect();
    Rect(const QPoint &topLeft, const QSize &size);
    Rect(const QRect &rect);

    int count();
    QPoint pos(bool isAbsolutePosition = true);
    bool next();
    int index();

private:
    bool isCounting;
    QPoint position;
};

#endif // RECT_H
