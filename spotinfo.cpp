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

    // If shape is empty(true) and actived is true, we should turn off actived mark
    // If shape is not empty(false) and active is false, we should turn on active mark
    // There's a little trick, when shape and active mark are same, we should reverse the active mark.
    if(m_shape.isEmpty() == m_actived)
    {
        m_actived = !m_actived;
        emit activedChanged(m_actived);
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

void SpotInfo::setPosition(QPoint &pos)
{
    QRect shape = m_shape;
    shape.moveTo(pos);
    setShape(shape);
}

void SpotInfo::initDatas()
{
    if(m_name.isEmpty())
        return;

    QSettings settings;
    if(settings.contains(key() + "shape"))
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
