#include "spotinfo.h"
#include <QSettings>
#include <QtDebug>

SpotInfo::SpotInfo(bool mandatory, QString name, QObject *parent) :
    QObject(parent),
    m_name(name),
    m_actived(false),
    m_mismatch(true),
    m_mandatory(mandatory),
    m_initialized(false)
{
    initDatas();
}

void SpotInfo::setShape(QRect shape)
{
    if (m_shape == shape)
        return;

    m_shape = shape;
    writeDatas();
    emit shapeChanged(m_shape);

    if(!m_shape.isEmpty())
    {
        if(!m_actived)
        {
            m_actived = true;
            emit activedChanged(m_actived);
        }
    }
    else
    {
        if(m_actived)
        {
            m_actived = false;
            emit activedChanged(m_actived);
        }
    }
}

void SpotInfo::setColors(QStringList colors)
{
    QString oldColors = m_colors.stringList().join(',');
    QString newColors = colors.join(',');
    if (oldColors == newColors)
        return;

    m_colors.setStringList(colors);
    writeDatas();
    emit colorsChanged();
}

void SpotInfo::initDatas()
{
    if(m_name.isEmpty())
        return;

    QSettings settings;
    if(settings.contains(key()))
        readDatas();
    else
        writeDatas();
    m_initialized = true;
}

QString SpotInfo::key()
{
    QString key = m_mandatory ? "spots/mandatory/" : "spots/optional/";
    key += m_name + "/";
    return key;
}

void SpotInfo::readDatas()
{
    QSettings settings;
    QRect shape = settings.value(key() + "shape", QRect()).toRect();
    setShape(shape);
    QStringList colors = settings.value(key() + "colors", QStringList()).toStringList();
    setColors(colors);
}

void SpotInfo::writeDatas()
{
    if(!m_initialized)
        return;

    QSettings settings;
    settings.setValue(key() + "shape", m_shape);
    settings.setValue(key() + "colors", m_colors.stringList());
}
