#ifndef SPOTINFO_H
#define SPOTINFO_H

#include <QObject>
#include <QStringListModel>
#include <QRect>

class SpotInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QRect shape READ shape WRITE setShape NOTIFY shapeChanged)
    Q_PROPERTY(bool actived READ actived NOTIFY activedChanged)
    Q_PROPERTY(bool mismatch READ mismatch NOTIFY mismatchChanged)

public:
    explicit SpotInfo(bool mandatory, QString name, QObject *parent = nullptr);

public:
    QString name() const
    {
        return m_name;
    }

    QRect shape() const
    {
        return m_shape;
    }

    bool actived() const
    {
        return m_actived;
    }

    QStringListModel *colors()
    {
        return &m_colors;
    }

    bool mismatch() const
    {
        return m_mismatch;
    }

signals:
    void shapeChanged(QRect shape);
    void activedChanged(bool actived);
    void mismatchChanged(bool mismatch);
    void colorsChanged();
    void nameChanged(QString name);

public slots:
    void setShape(QRect shape);
    void setColors(QStringList colors);
    void setPosition(QPoint &pos);

private:
    void initDatas();
    QString key();
    void readDatas();
    void writeDatas();

private:
    QString m_name;
    QRect m_shape;
    bool m_actived;
    QStringListModel m_colors;
    bool m_mismatch;

    bool m_mandatory;
    bool m_initialized;
};

#endif // SPOTINFO_H
