#include "singlecolor.h"

SingleColor::SingleColor(QString name, QScxmlStateMachine *machine, HotSpot *spot, QObject *parent) : QObject(parent),
    m_name(name),
    m_machine(machine),
    m_spot(spot)
{
    m_spotX = settings.value(QString("spots/") + name + "/x").toInt();
    m_spotY = settings.value(QString("spots/") + name + "/y").toInt();
    m_spotWidth = settings.value(QString("spots/") + name + "/width").toInt();
    m_spotHeight = settings.value(QString("spots/") + name + "/height").toInt();
    m_colorName = settings.value(QString("spots/") + name + "/colorName").toString();

    m_actived = checkActive();

    m_machine->connectToEvent(QLatin1String("HotSpot.Clicked"), this,
        [this](const QScxmlEvent &) {
        if( m_name != m_spot->name())
            return;
        else
            setSpot();
    });
}

void SingleColor::setSpot()
{

    if(m_spotX != m_spot->spotX())
    {
        m_spotX = m_spot->spotX();
        settings.setValue(QString("spots/") + m_name + "/x", m_spotX);
        emit spotXChanged(m_spotX);
    }

    if(m_spotY != m_spot->spotY())
    {
        m_spotY = m_spot->spotY();
        settings.setValue(QString("spots/") + m_name + "/y", m_spotY);
        emit spotYChanged(m_spotY);
    }

    if(m_spotWidth != m_spot->spotWidth())
    {
        m_spotWidth = m_spot->spotWidth();
        settings.setValue(QString("spots/") + m_name + "/width", m_spotWidth);
        emit spotWidthChanged(m_spotWidth);
    }

    if(m_spotHeight != m_spot->spotHeight())
    {
        m_spotHeight = m_spot->spotHeight();
        settings.setValue(QString("spots/") + m_name + "/height", m_spotHeight);
        emit spotHeightChanged(m_spotHeight);
    }

    bool active = checkActive();
    if(m_actived != active)
    {
        m_actived = active;
        emit activedChanged(m_actived);
    }

    if(m_colorName != m_spot->colorName())
    {
        m_colorName = m_spot->colorName();
        settings.setValue(QString("spots/") + m_name + "/colorName", m_colorName);
        emit colorNameChanged(m_colorName);
    }
}

bool SingleColor::checkActive()
{
    bool result;
    if(m_spotX>0 && m_spotY>0 && m_spotWidth>0 && m_spotHeight>0 && !m_name.isEmpty())
        result = true;
    else
        result = false;
    return result;
}
