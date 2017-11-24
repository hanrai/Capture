#ifndef HOTSPOT_H
#define HOTSPOT_H

#include <QObject>
#include <QScxmlStateMachine>
#include "ocr.h"

class HotSpot : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int spotX READ spotX NOTIFY spotXChanged)
    Q_PROPERTY(int spotY READ spotY NOTIFY spotYChanged)
    Q_PROPERTY(int spotWidth READ spotWidth NOTIFY spotWidthChanged)
    Q_PROPERTY(int spotHeight READ spotHeight NOTIFY spotHeightChanged)
    Q_PROPERTY(bool actived READ actived NOTIFY activedChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit HotSpot(Ocr *ocr, QScxmlStateMachine *machine, QObject *parent = nullptr);

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

    QString name() const
    {
        return m_name;
    }

signals:
    void spotXChanged(int spotX);

    void spotYChanged(int spotY);

    void spotWidthChanged(int spotWidth);

    void spotHeightChanged(int spotHeight);

    void activedChanged(bool actived);

    void nameChanged(QString name);

public slots:
    void setCursor(int x, int y);

    void setName(QString name)
    {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(m_name);
    }

private:
    int m_cursorX;
    int m_cursorY;
    int m_spotX;
    int m_spotY;
    int m_spotWidth;
    int m_spotHeight;
    bool m_actived;
    Ocr *m_ocr;
    QString m_name;
    QScxmlStateMachine *m_machine;
};

#endif // HOTSPOT_H
