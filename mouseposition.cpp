#include "mouseposition.h"
#include "mouseposition.h"

MousePosition::MousePosition(QObject *parent) : QObject(parent),
    x(0), y(0), w(0)
{
    x = settings.value("spots/mousePosition/x").toInt();
    y = settings.value("spots/mousePosition/y").toInt();
    w = settings.value("spots/mousePosition/w").toInt();
}

void MousePosition::setSpotX(int const p)
{
    if(x == p)
        return;
    x = p;
    settings.setValue("spots/mousePosition/x", x);
    emit spotXChanged();
}

int MousePosition::spotX() const
{
    return x;
}

void MousePosition::setSpotY(int const p)
{
    if(y == p)
        return;
    y = p;
    settings.setValue("spots/mousePosition/y", y);
    emit spotYChanged();
}

int MousePosition::spotY() const
{
    return y;
}

void MousePosition::setSpotRange(int const range)
{
    if(w == range)
        return;
    w = range;
    settings.setValue("spots/mousePosition/w", w);
    emit spotRangeChanged();
}

int MousePosition::spotRange() const
{
    return w;
}
