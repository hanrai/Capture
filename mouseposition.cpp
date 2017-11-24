#include "mouseposition.h"
#include "mouseposition.h"

MousePosition::MousePosition(QScxmlStateMachine *machine, QObject *parent) :
    QObject(parent),
    x(0), y(0), w(0), m_machine(machine)
{
    x = settings.value("spots/mousePosition/x").toInt();
    y = settings.value("spots/mousePosition/y").toInt();
    w = settings.value("spots/mousePosition/w").toInt();
    m_machine->connectToEvent(QLatin1String("SetMousePosition"), this,
            [this](const QScxmlEvent &event) {
            auto x = event.data().toMap().value("X").toInt();
            auto y = event.data().toMap().value("Y").toInt();
            setSpotX(x);
            setSpotY(y);
        });
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
