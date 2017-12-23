#ifndef HOTSPOT_H
#define HOTSPOT_H

#include <QObject>
#include <QScxmlStateMachine>
#include "ocr.h"

class HotSpot : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QRect shape READ shape WRITE setShape NOTIFY shapeChanged)
    Q_PROPERTY(bool actived READ actived WRITE setActived NOTIFY activedChanged)
    Q_PROPERTY(QString colorName READ colorName WRITE setColorName NOTIFY colorNameChanged)

public:
    explicit HotSpot(Ocr *ocr, QScxmlStateMachine *machine, QObject *parent = nullptr);

    bool actived() const
    {
        return m_actived;
    }

    QString colorName() const
    {
        return m_colorName;
    }

    QRect shape() const
    {
        return m_shape;
    }

signals:
    void activedChanged(bool actived);
    void colorNameChanged(QString colorName);
    void shapeChanged(QRect shape);

public slots:
    void setCursor(const QPoint &pos);
    void setColorName(QString colorName)
    {
        if (m_colorName == colorName)
            return;

        m_colorName = colorName;
        emit colorNameChanged(m_colorName);
    }

    void setShape(QRect shape)
    {
        if (m_shape == shape)
            return;

        m_shape = shape;
        emit shapeChanged(m_shape);
    }

    void setActived(bool actived)
    {
        if (m_actived == actived)
            return;

        m_actived = actived;
        emit activedChanged(m_actived);
    }

private:
    bool m_actived;
    Ocr *m_ocr;
    QString m_name;
    QScxmlStateMachine *m_machine;
    QString m_colorName;
    QRect m_shape;
};

#endif // HOTSPOT_H
