#ifndef MOUSEPOSITION_H
#define MOUSEPOSITION_H

#include <QObject>
#include <QSettings>
#include <QScxmlStateMachine>

class MousePosition : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int spotX READ spotX WRITE setSpotX NOTIFY spotXChanged)
    Q_PROPERTY(int spotY READ spotY WRITE setSpotY NOTIFY spotYChanged)
    Q_PROPERTY(int spotRange READ spotRange WRITE setSpotRange NOTIFY spotRangeChanged)

public:
    explicit MousePosition(QScxmlStateMachine *machine, QObject *parent = nullptr);

    void setSpotX(int const p);
    int spotX() const;
    void setSpotY(int const p);
    int spotY() const;
    void setSpotRange(int const range);
    int spotRange() const;

signals:
    void spotXChanged();
    void spotYChanged();
    void spotRangeChanged();

public slots:

private:
    int x;
    int y;
    int w;
    QSettings settings;
    QScxmlStateMachine *m_machine;
};

#endif // MOUSEPOSITION_H
