#include "hotspot.h"
#include <QColor>

HotSpot::HotSpot(Ocr *ocr, QScxmlStateMachine *machine, QObject *parent) : QObject(parent),
    m_spotX(0),
    m_spotY(0),
    m_spotWidth(0),
    m_spotHeight(0),
    m_actived(false),
    m_ocr(ocr),
    m_machine(machine),
    m_colorName("")
{
    m_machine->connectToEvent(QLatin1String("HotSpot.Move"), this,
        [this](const QScxmlEvent &event) {
        auto data = event.data().toMap();
        auto x = data.value("X").toInt();
        auto y = data.value("Y").toInt();
        auto name = data.value("NAME").toString();
        setCursor(x,y);
        setName(name);
    });

    m_machine->connectToEvent(QLatin1String("HotSpot.Disable"), this,
        [this](const QScxmlEvent &) {
        if(m_actived != false)
        {
            m_actived = false;
            emit activedChanged(m_actived);
        }
    });
}

void HotSpot::setCursor(int x, int y)
{
    if(m_ocr->Scan(QPoint(x, y)))
    {
        auto rect = m_ocr->getRect();
        if(m_spotX != rect.x())
        {
            m_spotX = rect.x();
            emit spotXChanged(m_spotX);
        }

        if(m_spotY != rect.y())
        {
            m_spotY = rect.y();
            emit spotYChanged(m_spotY);
        }

        if(m_spotWidth != rect.width())
        {
            m_spotWidth = rect.width();
            emit spotWidthChanged(m_spotWidth);
        }

        if(m_spotHeight != rect.height())
        {
            m_spotHeight = rect.height();
            emit spotHeightChanged(m_spotHeight);
        }

        if(m_actived != true)
        {
            m_actived = true;
            emit activedChanged(m_actived);
        }

        QString colorName = QColor(m_ocr->getColor()).name();
        if(m_colorName != colorName)
        {
            m_colorName = colorName;
            emit colorNameChanged(m_colorName);
        }
    }
    else
    {
        if(m_actived != false)
        {
            m_actived = false;
            emit activedChanged(m_actived);
        }
    }
}
