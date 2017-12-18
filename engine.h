#ifndef OCRENGINE_H
#define OCRENGINE_H

#include <QObject>
#include <ocr.h>
#include <QQuickImageProvider>
#include <QScxmlStateMachine>
#include "mouseposition.h"
#include "hotspot.h"
#include <QTime>
#include <QCoreApplication>
#include "singlecolor.h"
#include "desktopduplication.h"
#include <QtDebug>

class Engine : public QObject, public QQuickImageProvider
{
    Q_OBJECT
    Q_PROPERTY(QString snapshot READ snapshot NOTIFY snapshotChanged)
    Q_PROPERTY(bool snapshotLoaded READ snapshotLoaded NOTIFY snapshotLoadedChanged)
    Q_PROPERTY(QString actionName READ actionName WRITE setActionName NOTIFY actionNameChanged)

public:
    explicit Engine(QScxmlStateMachine* mc, QObject *parent = nullptr);

    QString snapshot() const {
        return snapName;
    }
    bool snapshotLoaded() const {
        return isSnapshotLoaded;
    }

    MousePosition *getMouseRanger() {return mp;}
    HotSpot *getHotSpot() {return m_hotSpot;}
    SingleColor *getContractSpot() {return m_contractSpot;}
    SingleColor *getDateSpot() {return m_dateSpot;}
    SingleColor *getTimeSpot() {return m_timeSpot;}

    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    QString actionName() const
    {
        return m_actionName;
    }

signals:
    void snapshotChanged();
    void snapshotLoadedChanged();

    void actionNameChanged(QString actionName);

public slots:
    void capture();

    void setActionName(QString actionName)
    {
        if (m_actionName == actionName)
            return;

        m_actionName = actionName;
        emit actionNameChanged(m_actionName);
        qDebug()<<"actionNameChangded:"<<m_actionName;
    }

private:
    void delay()
    {
        QTime dieTime= QTime::currentTime().addSecs(1);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

private:
    bool isSnapshotLoaded;
    MousePosition *mp;
    Ocr ocr;
    QString snapName;
    QScxmlStateMachine *machine;
    HotSpot *m_hotSpot;
    SingleColor *m_contractSpot;
    SingleColor *m_dateSpot;
    SingleColor *m_timeSpot;
    DesktopDuplication m_duplication;
    QString m_actionName;
};

#endif // OCRENGINE_H
