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
#include "spotinfo.h"
#include <QStringListModel>

class Engine : public QObject, public QQuickImageProvider
{
    Q_OBJECT
    Q_PROPERTY(QString snapshot READ snapshot NOTIFY snapshotChanged)
    Q_PROPERTY(bool snapshotLoaded READ snapshotLoaded NOTIFY snapshotLoadedChanged)
    Q_PROPERTY(QString action READ action WRITE setAction NOTIFY actionChanged)

public:
    explicit Engine(QScxmlStateMachine* mc, QObject *parent = nullptr);

    QString snapshot() const {
        return snapName;
    }
    bool snapshotLoaded() const {
        return isSnapshotLoaded;
    }

public slots:
    MousePosition *getMouseRanger() {return mp;}
    HotSpot *getHotSpot() {return m_hotSpot;}
    SingleColor *getContractSpot() {return m_contractSpot;}
    SingleColor *getDateSpot() {return m_dateSpot;}
    SingleColor *getTimeSpot() {return m_timeSpot;}

    SpotInfo *getSpotInfo(QString name);

public:
    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    QString action() const
    {
        return m_actionName;
    }

signals:
    void snapshotChanged();
    void snapshotLoadedChanged();

    void actionChanged(QString action);

public slots:
    void capture();

    void setAction(QString action);

private:
    void initMandatoryVectory();
    void initOptionalVectory();

private:
    bool isSnapshotLoaded;
    MousePosition *mp;
    Ocr ocr;
    QString snapName;
    QScxmlStateMachine *m_machine;
    HotSpot *m_hotSpot;
    SingleColor *m_contractSpot;
    SingleColor *m_dateSpot;
    SingleColor *m_timeSpot;
    DesktopDuplication m_duplication;
    QString m_actionName;
    QVector<SpotInfo*> m_mandatoryVector;
    QStringListModel m_optionalModel;
    QVector<SpotInfo*> m_optionalVectory;
    SpotInfo *m_activeSpot;
};

#endif // OCRENGINE_H
