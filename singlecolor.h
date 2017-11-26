#ifndef SINGLECOLOR_H
#define SINGLECOLOR_H

#include <QObject>
#include <QSettings>
#include <QScxmlStateMachine>
#include "hotspot.h"

class SingleColor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int spotX READ spotX NOTIFY spotXChanged)
    Q_PROPERTY(int spotY READ spotY NOTIFY spotYChanged)
    Q_PROPERTY(int spotWidth READ spotWidth NOTIFY spotWidthChanged)
    Q_PROPERTY(int spotHeight READ spotHeight NOTIFY spotHeightChanged)
    Q_PROPERTY(bool actived READ actived NOTIFY activedChanged)
    Q_PROPERTY(QString colorName READ colorName NOTIFY colorNameChanged)

public:
    explicit SingleColor(QString name, QScxmlStateMachine *machine, HotSpot *spot, QObject *parent = nullptr);

    int spotX() const
    {
        return m_spotX;
    }

    int spotY() const
    {
        return m_spotY;
    }

    int spotWidth() const
    {
        return m_spotWidth;
    }

    int spotHeight() const
    {
        return m_spotHeight;
    }

    bool actived() const
    {
        return m_actived;
    }

    QString colorName() const
    {
        return m_colorName;
    }

signals:
    void spotXChanged(int spotX);

    void spotYChanged(int spotY);

    void spotWidthChanged(int spotWidth);

    void spotHeightChanged(int spotHeight);

    void activedChanged(bool actived);

    void colorNameChanged(QString colorName);

public slots:
    void setSpot();
    bool checkActive();

private:
    int m_cursorX;
    int m_cursorY;
    int m_spotX;
    int m_spotY;
    int m_spotWidth;
    int m_spotHeight;
    bool m_actived;
    QString m_name;
    QString m_colorName;
    QSettings settings;
    QScxmlStateMachine *m_machine;
    HotSpot *m_spot;
};

#endif // SINGLECOLOR_H
