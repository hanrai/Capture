#ifndef OCRENGINE_H
#define OCRENGINE_H

#include <QObject>
#include <QQuickImageProvider>
#include <QScxmlStateMachine>
#include <QTime>
#include <QCoreApplication>
#include <QtDebug>
#include <QStringListModel>
#include "desktopduplication.h"
#include "ocr.h"
#include "hotspot.h"
#include "spotinfo.h"

class Engine : public QObject, public QQuickImageProvider
{
    Q_OBJECT
    Q_PROPERTY(QString snapshot READ snapshot NOTIFY snapshotChanged)
    Q_PROPERTY(bool snapshotLoaded READ snapshotLoaded NOTIFY snapshotLoadedChanged)
    Q_PROPERTY(QString action READ action WRITE setAction NOTIFY actionChanged)

public:
    explicit Engine(QScxmlStateMachine* mc, QObject *parent = nullptr);

    QString snapshot() const {
        return m_snapshotName;
    }
    bool snapshotLoaded() const {
        return m_snapshotLoaded;
    }

public slots:
    HotSpot *getHotSpot() {return m_hotSpot;}
    SpotInfo *getSpotInfo(QString name);

public:
    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
    QString action() const;

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
    bool m_snapshotLoaded;
    Ocr m_ocr;
    QString m_snapshotName;
    QScxmlStateMachine *m_machine;
    HotSpot *m_hotSpot;
    DesktopDuplication m_duplication;
    QVector<SpotInfo*> m_mandatoryVector;
    QStringListModel m_optionalModel;
    QVector<SpotInfo*> m_optionalVectory;
    SpotInfo *m_activeSpot;
    QImage m_snapShot;
};

#endif // OCRENGINE_H
