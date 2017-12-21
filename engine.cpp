#include "engine.h"
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QImage>
#include <QCursor>
#include <QUuid>
#include <QtDebug>
#include <QElapsedTimer>

Engine::Engine(QScxmlStateMachine *machine, QObject *parent) :
    QObject(parent),
    QQuickImageProvider(QQuickImageProvider::Image),
    isSnapshotLoaded(false),
    m_machine(machine)
{
    mp = new MousePosition(machine, this);
    m_hotSpot = new HotSpot(&ocr, machine, this);
    m_contractSpot = new SingleColor("Contract", m_machine, m_hotSpot, this);
    m_dateSpot = new SingleColor("Date", m_machine, m_hotSpot, this);
    m_timeSpot = new SingleColor("Time", m_machine, m_hotSpot, this);

    initMandatoryVectory();

    m_machine->connectToEvent(QLatin1String("Action.Changged"), this,
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

void Engine::initMandatoryVectory()
{
    m_mandatoryVector.push_back(new SpotInfo(true, "鼠标指针", this));
    m_mandatoryVector.push_back(new SpotInfo(true, "合约", this));
    m_mandatoryVector.push_back(new SpotInfo(true, "日期", this));
    m_mandatoryVector.push_back(new SpotInfo(true, "时间", this));
    m_mandatoryVector.push_back(new SpotInfo(true, "点位", this));
}
