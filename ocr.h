#ifndef OCR_H
#define OCR_H

#include <QObject>
#include <QHash>
#include <QBitArray>
#include <QImage>
#include "rect.h"
struct Image
{
    Image():m_rowPitch(0),m_depthPitch(0),m_buffer(nullptr) {}

    QRect rect() {return m_rect;}
    QRgb pixel(QPoint pos) {return *(m_buffer+pos.x()+pos.y()*m_rowPitch/4);}

    QRect m_rect;
    quint32 m_rowPitch;
    quint32 m_depthPitch;
    QRgb *m_buffer;
};

class Ocr : public QObject
{
    Q_OBJECT
public:
    explicit Ocr(QObject *parent = nullptr);

signals:

public:
    void setImageInfo(QRgb* buffer, QSize size, unsigned int rowPitch, unsigned int depthPitch);
    bool Scan(QPoint pos);
    QString getString();
    QRect getRect();
    int getScale();
    QRgb getColor();

private:
    bool testChar(const QSize &sz);
    void seekingBackward();
    bool decodeString();
    bool decode(const QPoint &pos, const QSize &sz);
    QHash<QRgb, QBitArray> getSamples(const QRect &sampleArea);
    QBitArray getSample(const QRect &sampleArea);
    bool testChar(const QHash<QRgb, QBitArray> &samples);
    static QBitArray initBitArray(const QVector<bool> &pattern);

private:
    QHash<QBitArray,char> charPattern;

    char c;
    int scale;
    QRgb color;
    QSize charSize;
    QSize chineseSize;
    QPoint cursorPos;
    QPoint originPos;
    QString string;
    QRect stringRect;

    Image image;
};

#endif // OCR_H
