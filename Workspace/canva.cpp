#include <QSettings>
#include <QPainter>
#include <QPaintEvent>
#include <QGraphicsPixmapItem>
#include "canva.h"
#include "CanvaMachine.h"

Canva::Canva(QWidget *parent) :
    QGraphicsView(parent),
    m_machine(new CanvaMachine(this)),
    m_scale(1),
    m_imageItem(nullptr),
    m_scene(new QGraphicsScene)
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setMouseTracking(true);

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

    if(m_imageItem == nullptr)
    {
        m_imageItem = new QGraphicsPixmapItem;
        m_imageItem->setTransformationMode(Qt::SmoothTransformation);
        m_scene->addItem(m_imageItem);
    }

    m_imageItem->setPixmap(QPixmap::fromImage(m_image));

    m_machine->submitEvent("ImageChanged");
    emit imageChanged(m_image);
}

//void Canva::mouseDoubleClickEvent(QMouseEvent *event)
//{

//}

//void Canva::mouseMoveEvent(QMouseEvent *event)
//{

//}

//void Canva::mousePressEvent(QMouseEvent *event)
//{

//}

//void Canva::mouseReleaseEvent(QMouseEvent *event)
//{

//}

//void Canva::wheelEvent(QWheelEvent *event)
//{

//}

//void Canva::resizeEvent(QResizeEvent *event)
//{

//}

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
