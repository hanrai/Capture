#include "hotspot.h"
#include <QColor>
#include <QtDebug>

HotSpot::HotSpot(Ocr *ocr, QScxmlStateMachine *machine, QObject *parent) : QObject(parent),
    m_actived(false),
    m_ocr(ocr),
    m_machine(machine)
{
    m_machine->connectToEvent(QLatin1String("HotSpot.Moved"), this,
        [this](const QScxmlEvent &event) {
        auto data = event.data().toMap();
        auto pos = data.value("pos").toPoint();
        setCursor(pos);
    });

    m_machine->connectToEvent(QLatin1String("HotSpot.Disable"), this,
        [this](const QScxmlEvent &) {
        setActived(false);
    });
}

void HotSpot::setCursor(const QPoint &pos)
{
    if(m_ocr->Scan(pos))
    {
        auto rect = m_ocr->getRect();

        setShape(rect);
        setActived(true);

        QString colorName = QColor(m_ocr->getColor()).name();
        setColorName(colorName);
    }
    else
    {
        setActived(false);
    }
}
