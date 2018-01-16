#ifndef CANVASTATUS_H
#define CANVASTATUS_H

#include <QWidget>
class QLabel;
class Canva;

class CanvaStatus : public QWidget
{
    Q_OBJECT
public:
    explicit CanvaStatus(Canva *canva, QWidget *parent = nullptr);

signals:

public slots:

private slots:
    void updateInfo();

private:
    Canva *m_canva;
    QLabel *m_colorIndicator;
    QLabel *m_colorName;
    QLabel *m_position;
};

#endif // CANVASTATUS_H
