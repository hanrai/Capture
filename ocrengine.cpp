#include "ocrengine.h"
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QImage>
#include <QUuid>

OCREngine::OCREngine(QScxmlStateMachine *mc, QObject *parent) :
    QObject(parent),
    QQuickImageProvider(QQuickImageProvider::Image),
    isLoaded(false),
    machine(mc)
{
    mp = new MousePosition(this);
}

QImage OCREngine::requestImage(const QString &, QSize *size, const QSize &)
{
    if(size)
        *size = ocr.image.size();
    return ocr.image;
}

void OCREngine::capture()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    auto pix = screen->grabWindow(0);
    ocr.image = pix.toImage();
    snapName = QUuid::createUuid().toString();

    if(!isLoaded)
    {
        isLoaded = true;
        emit loadedChanged();
    }

    emit snapshotChanged();
}
