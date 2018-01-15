#ifndef CANVA_H
#define CANVA_H

#include <QGraphicsView>
#include <QImage>
#include <QtDebug>

class CanvaMachine;
class QGraphicsScene;

class Canva : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(bool imageLoaded READ imageLoaded NOTIFY imageLoadedChanged)
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

signals:
    void imageChanged(QImage image);
    void imageLoadedChanged(bool imageLoaded);

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

protected:
//    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
//    void wheelEvent(QWheelEvent *event) override;
//    void resizeEvent(QResizeEvent *event) override;

private:
    void resetBackgroundPattern();
    void setupMachine();
    void submitMouseEvent(QString name, QMouseEvent *event) const;

private:
    CanvaMachine *m_machine;
    QImage m_image;
    qreal m_scale;
    QGraphicsPixmapItem *m_imageItem;
    QGraphicsScene *m_scene;
    bool m_imageLoaded;
};

#endif // CANVA_H
