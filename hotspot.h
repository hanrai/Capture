#ifndef HOTSPOT_H
#define HOTSPOT_H

#include <QObject>

class HotSpot : public QObject
{
    Q_OBJECT
public:
    explicit HotSpot(QObject *parent = nullptr);

signals:

public slots:
};

#endif // HOTSPOT_H