#ifndef CANVA_H
#define CANVA_H

#include <QGraphicsView>
#include <QImage>

class CanvaMachine;
class QGraphicsScene;

class Canva : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
public:
    explicit Canva(QWidget *parent = nullptr);
    ~Canva();

    QImage image() const
    {
        return m_image;
    }

signals:
    void imageChanged(QImage image);

public slots:
    void setImage(QImage image);

protected:
//    void mouseDoubleClickEvent(QMouseEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override;
//    void mousePressEvent(QMouseEvent *event) override;
//    void mouseReleaseEvent(QMouseEvent *event) override;
//    void wheelEvent(QWheelEvent *event) override;
//    void resizeEvent(QResizeEvent *event) override;

private:
    void resetBackgroundPattern();

private:
    CanvaMachine *m_machine;
    QImage m_image;
    qreal m_scale;
    QRect m_sourceRect;
    QGraphicsPixmapItem *m_imageItem;
    QGraphicsScene *m_scene;
};

#endif // CANVA_H
