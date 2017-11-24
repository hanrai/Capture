#include "engine.h"
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QImage>
#include <QUuid>

Engine::Engine(QScxmlStateMachine *mc, QObject *parent) :
    QObject(parent),
    QQuickImageProvider(QQuickImageProvider::Image),
    isSnapshotLoaded(false),
    machine(mc)
{
    mp = new MousePosition(mc, this);
    m_hotSpot = new HotSpot(&ocr, mc, this);
}

QImage Engine::requestImage(const QString &, QSize *size, const QSize &)
{
    if(size)
        *size = ocr.image.size();
    return ocr.image;
}

void Engine::capture()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    auto pix = screen->grabWindow(0);
    ocr.image = pix.toImage();
    snapName = QUuid::createUuid().toString();

    if(!isSnapshotLoaded)
    {
        isSnapshotLoaded = true;
        emit snapshotLoadedChanged();
    }
    emit snapshotChanged();
}
