#include "rect.h"

Rect::Rect():QRect(),isCounting(false)
{

}

Rect::Rect(const QPoint &topLeft, const QSize &size):
    QRect(topLeft, size)
{
    Rect();
}

Rect::Rect(const QRect &rect):
    QRect(rect)
{
    Rect();
}

int Rect::count()
{
    return width()*height();
}

QPoint Rect::pos(bool isAbsolutePosition)
{
    if(isAbsolutePosition)
        return topLeft()+position;
    else
        return position;
}

bool Rect::next()
{
    if(!isCounting)
    {
        isCounting = true;
        return true;
    }

    if(position.y()*width()+position.x()+1 >= count())
        return false;

    position.rx()++;

    if(position.x() >= width())
    {
        position.setX(0);
        position.ry()++;
    }

    return true;
}

int Rect::index()
{
    return position.y()*width()+position.x();
}
