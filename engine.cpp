#include "engine.h"
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QImage>
#include <QCursor>
#include <QUuid>
#include <QtDebug>
#include <QElapsedTimer>

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

    machine->connectToEvent(QLatin1String("Action.Changged"), this,
        [this](const QScxmlEvent &event) {
        auto data = event.data().toMap();
        auto name = data.value("ActionName").toString();
        setActionName(name);
    });
}

QImage Engine::requestImage(const QString &, QSize *size, const QSize &)
{
    if(size)
        *size = ocr.image.size();
    return ocr.image;
}

void Engine::capture()
{
    SnapshotInfo info;
    m_duplication.beginCapture(0);
    QElapsedTimer timer;
    timer.start();

    double c=1;

    for(int i=0; i<c; i++)
    {
        m_duplication.capture(info);
    }

    qDebug() << "FPS:" << 1000*c/(timer.elapsed());
    QImage img(info.width, info.height, QImage::Format_ARGB32);
    for(int i=0; i<img.height(); i++)
    {
        memcpy(
            (void*)((char*)img.bits()+i*img.width()*4),
            (void*)((char*)info.buffer+i*info.pitch),
            img.width()*4);
    }
    m_duplication.endCapture();

    snapName = QUuid::createUuid().toString();
    ocr.image = img;
    if(!isSnapshotLoaded)
    {
        isSnapshotLoaded = true;
        emit snapshotLoadedChanged();
    }
    emit snapshotChanged();
}
