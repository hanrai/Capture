#ifndef CANVA_H
#define CANVA_H

#include <QWidget>
#include <QImage>

class CanvaMachine;

class Canva : public QWidget
{
    Q_OBJECT
public:
    explicit Canva(QWidget *parent = nullptr);
    ~Canva();

    void setImage(QImage image);

signals:

public slots:

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void resetBackgroundPattern();

private:
    CanvaMachine *m_machine;
    QImage m_image;
    QPixmap m_backgroundTile;
    qreal m_scale;
};

#endif // CANVA_H
