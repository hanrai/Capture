#ifndef OCRENGINE_H
#define OCRENGINE_H

#include <QObject>
#include <ocr.h>
#include <QQuickImageProvider>
#include <QScxmlStateMachine>
#include "mouseposition.h"

class OCREngine : public QObject, public QQuickImageProvider
{
    Q_OBJECT
    Q_PROPERTY(QString snapshot READ snapshot NOTIFY snapshotChanged)
    Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged)

public:
    explicit OCREngine(QScxmlStateMachine* mc, QObject *parent = nullptr);

    QString snapshot() const {
        return snapName;
    }
    bool loaded() const {
        return isLoaded;
    }

    MousePosition *getMouseRanger() {return mp;}

    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

signals:
    void snapshotChanged();
    void loadedChanged();

public slots:
    void capture();

private:
    bool isLoaded;
    MousePosition *mp;
    Ocr ocr;
    QString snapName;
    QScxmlStateMachine *machine;
};

#endif // OCRENGINE_H
