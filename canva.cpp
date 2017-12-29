#include <QSettings>
#include <QPainter>
#include <QPaintEvent>
#include "canva.h"
#include "CanvaMachine.h"

Canva::Canva(QWidget *parent) :
    QWidget(parent),
    m_machine(new CanvaMachine(this)),
    m_scale(1)
{
    m_machine->start();
    setMouseTracking(true);
    resetBackgroundPattern();
}

Canva::~Canva()
{

}

void Canva::setImage(QImage image)
{
    m_image = image;
    m_machine->submitEvent("ImageChanged");
}

void Canva::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void Canva::mouseMoveEvent(QMouseEvent *event)
{

}

void Canva::mousePressEvent(QMouseEvent *event)
{

}

void Canva::mouseReleaseEvent(QMouseEvent *event)
{

}

void Canva::wheelEvent(QWheelEvent *event)
{

}

void Canva::resizeEvent(QResizeEvent *event)
{

}

void Canva::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    painter.drawTiledPixmap(event->rect(), m_backgroundTile);
}

void Canva::resetBackgroundPattern()
{
    QSettings settings;
    settings.beginGroup("Canva/Background");
    QSize cellSize = settings.value("CellSize", QSize(30, 30)).toSize();
    QRgb lighterColor = settings.value("LighterColor", 0xFFD3D3D3).toUInt();
    QRgb darkerColor = settings.value("DarkerColor", 0xFFA9A9A9).toUInt();

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

    m_backgroundTile = tile;
}
