#ifndef OCRENGINE_H
#define OCRENGINE_H

#include <QObject>
#include <ocr.h>
#include <QQuickImageProvider>
#include <QScxmlStateMachine>
#include "mouseposition.h"
#include "hotspot.h"

class Engine : public QObject, public QQuickImageProvider
{
    Q_OBJECT
    Q_PROPERTY(QString snapshot READ snapshot NOTIFY snapshotChanged)
    Q_PROPERTY(bool snapshotLoaded READ snapshotLoaded NOTIFY snapshotLoadedChanged)

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

    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

signals:
    void snapshotChanged();
    void snapshotLoadedChanged();

public slots:
    void capture();

private:
    bool isSnapshotLoaded;
    MousePosition *mp;
    Ocr ocr;
    QString snapName;
    QScxmlStateMachine *machine;
    HotSpot *m_hotSpot;
};

#endif // OCRENGINE_H
