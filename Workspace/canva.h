#ifndef CANVA_H
#define CANVA_H

#include <QGraphicsView>
#include <QImage>
#include <QPoint>
#include <QColor>
#include <QtDebug>

class CanvaMachine;
class QGraphicsScene;

class Canva : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(bool imageLoaded READ imageLoaded NOTIFY imageLoadedChanged)
    Q_PROPERTY(QPoint position READ position NOTIFY positionChanged)
    Q_PROPERTY(QColor color READ color NOTIFY colorChanged)
public:
    explicit Canva(QWidget *parent = nullptr);
    ~Canva();

    QImage image() const
    {
        return m_image;
    }

    bool imageLoaded() const
    {
        return m_imageLoaded;
    }

    QPoint position() const
    {
        return m_position;
    }

    QColor color() const
    {
        return m_color;
    }

signals:
    void imageChanged(QImage image);
    void imageLoadedChanged(bool imageLoaded);
    void positionChanged(QPoint position);
    void colorChanged(QColor color);

public slots:
    void setImage(QImage image);

private slots:
    void setImageLoaded(bool imageLoaded)
    {
        if (m_imageLoaded == imageLoaded)
            return;
        m_imageLoaded = imageLoaded;
        emit imageLoadedChanged(m_imageLoaded);
    }

    void setPosition(QPoint position);
    void setColor(QPoint position);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void resetBackgroundPattern();
    void setupMachine();
    void submitMouseEvent(QString name, QMouseEvent *event) const;

private:
    CanvaMachine *m_machine;
    QImage m_image;
    QGraphicsPixmapItem *m_imageItem;
    QGraphicsScene *m_scene;
    bool m_imageLoaded;
    QPoint m_position;
    QColor m_color;
};

#endif // CANVA_H
