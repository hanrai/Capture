#ifndef OCR_H
#define OCR_H

#include <QObject>
#include <QHash>
#include <QBitArray>
#include <QImage>
#include "rect.h"

class Ocr : public QObject
{
    Q_OBJECT
public:
    explicit Ocr(QObject *parent = nullptr);
    QImage image;

signals:

public:
    bool Scan(QPoint pos);
    QString getString();
    QRect getRect();
    int getScale();

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
};

#endif // OCR_H
