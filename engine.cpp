#include "engine.h"
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QImage>
#include <QCursor>
#include <QUuid>
#include <QtDebug>
#include <QElapsedTimer>
#include <QSettings>

Engine::Engine(QScxmlStateMachine *machine, QObject *parent) :
    QObject(parent),
    QQuickImageProvider(QQuickImageProvider::Image),
    m_snapshotLoaded(false),
    m_machine(machine),
    m_activeSpot(nullptr)
{
    m_hotSpot = new HotSpot(&m_ocr, machine, this);

    initMandatoryVectory();
    initOptionalVectory();

    m_machine->connectToEvent(QLatin1String("Action.Changged"), this,
        [this](const QScxmlEvent &event) {
        auto data = event.data().toMap();
        auto name = data.value("ActionName").toString();
        setAction(name);
    });

    m_machine->connectToEvent(QLatin1String("SetMousePosition"), this,
        [this](const QScxmlEvent &event) {
        auto data = event.data().toMap();
        auto pos = data.value("pos").toPoint();
        if(m_activeSpot == nullptr)
            return;
        m_activeSpot->setPosition(pos);
    });

    m_machine->connectToEvent(QLatin1String("SetSpotData"), this,
        [this](const QScxmlEvent &) {
        Q_ASSERT(m_activeSpot);
        m_activeSpot->setShape(m_hotSpot->shape());
        //TODO: add color to color list
    });
}

SpotInfo *Engine::getSpotInfo(QString name)
{
    foreach(SpotInfo *info, m_mandatoryVector + m_optionalVectory)
        if(info->name() == name)
            return info;

    return nullptr;
}

QImage Engine::requestImage(const QString &, QSize *size, const QSize &)
{
    if(size)
        *size = m_snapShot.size();
    return m_snapShot;
}

QString Engine::action() const
{
    if(m_activeSpot == nullptr)
        return QString();
    return m_activeSpot->name();
}

void Engine::capture()
{
    QElapsedTimer timer;
    timer.start();

    double c=1;

    for(int i=0; i<c; i++)
    {
        m_snapShot = m_duplication.takeSnapshot(0);
    }

    qDebug() << "FPS:" << 1000*c/(timer.elapsed());

    m_snapshotName = QUuid::createUuid().toString();
    m_ocr.setImageInfo((QRgb*)m_snapShot.bits(),
                     m_snapShot.size(),
                     m_snapShot.width()*4,
                     m_snapShot.width()*4*m_snapShot.height());

    if(!m_snapshotLoaded)
    {
        m_snapshotLoaded = true;
        emit snapshotLoadedChanged();
    }
    emit snapshotChanged();
}

void Engine::setAction(QString actionName)
{
    if(actionName.isEmpty())
    {
        if(m_activeSpot != nullptr)
        {
            m_activeSpot = nullptr;
            emit actionChanged(actionName);
        }
        return;
    }

    if(m_activeSpot != nullptr)
    {
        if(m_activeSpot->name() == actionName)
            return;
    }

    m_activeSpot = getSpotInfo(actionName);
    Q_ASSERT(m_activeSpot);
    emit actionChanged(actionName);
}

void Engine::initMandatoryVectory()
{
    m_mandatoryVector.push_back(new SpotInfo(true, "鼠标位置", this));
    m_mandatoryVector.push_back(new SpotInfo(true, "合约", this));
    m_mandatoryVector.push_back(new SpotInfo(true, "日期", this));
    m_mandatoryVector.push_back(new SpotInfo(true, "时间", this));
    m_mandatoryVector.push_back(new SpotInfo(true, "点位", this));
}

void Engine::initOptionalVectory()
{
    QSettings settings;
    settings.beginGroup("spots/optional");

    QStringList groups = settings.childGroups();
    m_optionalModel.setStringList(groups);

    foreach(auto group, groups)
    {
        SpotInfo *info = new SpotInfo(false, group, this);
        m_optionalVectory.push_back(info);
    }
    settings.endGroup();
}
