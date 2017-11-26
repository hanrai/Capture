#include "engine.h"
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QImage>
#include <QCursor>
#include <QUuid>
#include <QtDebug>

Engine::Engine(QScxmlStateMachine *mc, QObject *parent) :
    QObject(parent),
    QQuickImageProvider(QQuickImageProvider::Image),
    isSnapshotLoaded(false),
    machine(mc)
{
    mp = new MousePosition(mc, this);
    m_hotSpot = new HotSpot(&ocr, mc, this);
    m_contractSpot = new SingleColor("Contract", machine, m_hotSpot, this);
    m_dateSpot = new SingleColor("Date", machine, m_hotSpot, this);
    m_timeSpot = new SingleColor("Time", machine, m_hotSpot, this);
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
