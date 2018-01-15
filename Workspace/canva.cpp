#include <QSettings>
#include <QPainter>
#include <QPaintEvent>
#include <QGraphicsPixmapItem>
#include <QScrollBar>
#include "canva.h"
#include "CanvaMachine.h"

Canva::Canva(QWidget *parent) :
    QGraphicsView(parent),
    m_machine(new CanvaMachine(this)),
    m_scale(1),
    m_imageItem(nullptr),
    m_scene(new QGraphicsScene),
    m_imageLoaded(false)
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setMouseTracking(true);
    setupMachine();

    m_machine->start();
    resetBackgroundPattern();
}

Canva::~Canva()
{

}

void Canva::setImage(QImage image)
{
    if(m_image == image)
        return;

    QTransform transform;
    transform.reset();
    setTransform(transform);

    m_image = image;

    if(image.isNull()) {
        Q_ASSERT(m_imageItem);
        m_scene->removeItem(m_imageItem);
        delete m_imageItem;
        m_imageItem = nullptr;
        setImageLoaded(false);
    } else {
        if(m_imageItem == nullptr) {
            m_imageItem = new QGraphicsPixmapItem;
            m_imageItem->setTransformationMode(Qt::SmoothTransformation);
            m_scene->addItem(m_imageItem);
        }
        m_imageItem->setPixmap(QPixmap::fromImage(m_image));
        setImageLoaded(true);
    }

    m_machine->submitEvent("ImageChanged");
    emit imageChanged(m_image);
}

void Canva::mouseMoveEvent(QMouseEvent *event)
{
    submitMouseEvent("Mouse.Moved", event);
    QGraphicsView::mouseMoveEvent(event);
}

void Canva::mousePressEvent(QMouseEvent *event)
{
    submitMouseEvent("Mouse.Pressed", event);
    QGraphicsView::mousePressEvent(event);
}

void Canva::mouseReleaseEvent(QMouseEvent *event)
{
    submitMouseEvent("Mouse.Released", event);
    QGraphicsView::mouseReleaseEvent(event);
}

void Canva::wheelEvent(QWheelEvent *event)
{
    m_machine->submitEvent("Mouse.Wheel", QVariantMap({
            { "x", event->x() },
            { "y", event->y() },
            { "dx", event->angleDelta().x() },
            { "dy", event->angleDelta().y() }
        }));
}

void Canva::resetBackgroundPattern()
{
    QSettings settings;
    settings.beginGroup("Canva/Background");
    QSize cellSize = settings.value("CellSize", QSize(25, 25)).toSize();
    QRgb lighterColor = settings.value("LighterColor", 0xFFD3D3D3).toUInt();
    QRgb darkerColor = settings.value("DarkerColor", 0xFFA9A9A9).toUInt();
    settings.endGroup();

    QPixmap tile(cellSize * 2);
    tile.fill(lighterColor);

    QRect cellRect(QPoint(0, 0), cellSize);
    QBrush cellBrush(QColor::fromRgb(darkerColor));

    QPainter painter(&tile);
    painter.fillRect(cellRect, cellBrush);
    cellRect.translate(cellRect.bottomRight());
    cellRect.translate(1, 1);
    painter.fillRect(cellRect, cellBrush);
    painter.end();

    QPalette palette;
    palette.setBrush(QPalette::Base, tile);
    setPalette(palette);

    update();
}

void Canva::setupMachine()
{
    QSettings settings;
    settings.beginGroup("Canva/Interactive");
    auto button = settings.value("ResetScaleButton", 4).toInt();
    auto scaleStep = settings.value("ScaleStep", 6).toDouble();
    settings.endGroup();

    m_machine->setInitialValues(QVariantMap({
                { "ResetScaleButton", button},
                { "Step", scaleStep}
            }));

    connect(this, &Canva::imageLoadedChanged, [this](const bool loaded){
        m_machine->submitEvent(loaded ? "ImageLoaded" : "ImageUnloaded");
    });

    m_machine->connectToState("Movable.Set.Move", this,
        [this](const bool isDraging) {
        setCursor(isDraging ? Qt::ClosedHandCursor : Qt::ArrowCursor);
    });

    m_machine->connectToEvent("Draging", this,
        [this](const QScxmlEvent &event) {
        auto dx = event.data().toMap().value("x").toInt();
        auto dy = event.data().toMap().value("y").toInt();
        auto *scrollBar = horizontalScrollBar();
        scrollBar->setValue(scrollBar->value() - dx);
        scrollBar = verticalScrollBar();
        scrollBar->setValue(scrollBar->value() - dy);
    });

    m_machine->connectToEvent("Scaling", this,
        [this](const QScxmlEvent &event) {
        auto scale = event.data().toMap().value("scale").toDouble();
        QTransform transform;
        transform.scale(scale, scale);
        setTransform(transform);
    });
}

void Canva::submitMouseEvent(QString name, QMouseEvent *event) const
{
    m_machine->submitEvent(name, QVariantMap({
            { "x", event->pos().x() },
            { "y", event->pos().y() },
            { "button", event->button()}
        }));
}
