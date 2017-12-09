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
}

QImage Engine::requestImage(const QString &, QSize *size, const QSize &)
{
    if(size)
        *size = ocr.image.size();
    return ocr.image;
}

void Engine::capture()
{
    qDebug()<<m_duplication.getOutputCount();
    m_duplication.setOutputID(0);

    QElapsedTimer timer;
    timer.start();

    for(int i=0; i<1000; i++)
    {
        ocr.image = m_duplication.takeSnapshot();
        qDebug()<<i;
    }

    qDebug() << "FPS:" << timer.elapsed();

    snapName = QUuid::createUuid().toString();

    if(!isSnapshotLoaded)
    {
        isSnapshotLoaded = true;
        emit snapshotLoadedChanged();
    }
    emit snapshotChanged();
}
