#include "ocr.h"
#include <QBitArray>
#include <QtDebug>

Ocr::Ocr(QObject *parent) : QObject(parent)
{
    charSize.setWidth(6);
    charSize.setHeight(12);
    chineseSize.setWidth(12);
    chineseSize.setHeight(12);

    QVector<bool> p0 = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,1,1,1,0,0,
        1,0,0,0,1,0,
        1,0,0,0,1,0,
        1,0,0,0,1,0,
        1,0,0,0,1,0,
        1,0,0,0,1,0,
        1,0,0,0,1,0,
        0,1,1,1,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray i0 = initBitArray(p0);

    QVector<bool> p1 = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,1,0,0,0,
        0,1,1,0,0,0,
        0,0,1,0,0,0,
        0,0,1,0,0,0,
        0,0,1,0,0,0,
        0,0,1,0,0,0,
        0,0,1,0,0,0,
        0,1,1,1,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray i1 = initBitArray(p1);

    QVector<bool> p2 = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,1,1,1,0,0,
        1,0,0,0,1,0,
        1,0,0,0,1,0,
        0,0,0,1,0,0,
        0,0,1,0,0,0,
        0,1,0,0,0,0,
        1,0,0,0,0,0,
        1,1,1,1,1,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray i2 = initBitArray(p2);

    QVector<bool> p3 = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,1,1,1,0,0,
        1,0,0,0,1,0,
        0,0,0,0,1,0,
        0,0,1,1,0,0,
        0,0,0,0,1,0,
        0,0,0,0,1,0,
        1,0,0,0,1,0,
        0,1,1,1,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray i3 = initBitArray(p3);

    QVector<bool> p4 = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,1,0,0,
        0,0,1,1,0,0,
        0,0,1,1,0,0,
        0,1,0,1,0,0,
        1,0,0,1,0,0,
        1,1,1,1,1,0,
        0,0,0,1,0,0,
        0,0,1,1,1,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray i4 = initBitArray(p4);

    QVector<bool> p5 = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        1,1,1,1,1,0,
        1,0,0,0,0,0,
        1,0,0,0,0,0,
        1,1,1,1,0,0,
        1,0,0,0,1,0,
        0,0,0,0,1,0,
        1,0,0,0,1,0,
        0,1,1,1,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray i5 = initBitArray(p5);

    QVector<bool> p6 = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,1,1,0,0,
        0,1,0,0,1,0,
        1,0,0,0,0,0,
        1,0,1,1,0,0,
        1,1,0,0,1,0,
        1,0,0,0,1,0,
        1,0,0,0,1,0,
        0,1,1,1,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray i6 = initBitArray(p6);

    QVector<bool> p7 = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,1,1,1,1,0,
        0,0,0,0,1,0,
        0,0,0,1,0,0,
        0,0,0,1,0,0,
        0,0,1,0,0,0,
        0,0,1,0,0,0,
        0,0,1,0,0,0,
        0,0,1,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray i7 = initBitArray(p7);

    QVector<bool> p8 = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,1,1,1,0,0,
        1,0,0,0,1,0,
        1,0,0,0,1,0,
        0,1,1,1,0,0,
        1,0,0,0,1,0,
        1,0,0,0,1,0,
        1,0,0,0,1,0,
        0,1,1,1,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray i8 = initBitArray(p8);

    QVector<bool> p9 = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,1,1,1,0,0,
        1,0,0,0,1,0,
        1,0,0,0,1,0,
        1,0,0,1,1,0,
        0,1,1,0,1,0,
        0,0,0,0,1,0,
        1,0,0,1,0,0,
        0,1,1,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray i9 = initBitArray(p9);

    QVector<bool> pm = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        1,1,1,1,1,1,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray im = initBitArray(pm);

    QVector<bool> pp = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,1,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray ip = initBitArray(pp);

    QVector<bool> pc = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,1,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,1,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray ic = initBitArray(pc);

    QVector<bool> ps = {
        0,0,0,0,0,0,
        0,0,0,0,0,1,
        0,0,0,0,1,0,
        0,0,0,0,1,0,
        0,0,0,1,0,0,
        0,0,0,1,0,0,
        0,0,1,0,0,0,
        0,0,1,0,0,0,
        0,1,0,0,0,0,
        0,1,0,0,0,0,
        1,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray is = initBitArray(ps);

    QVector<bool> pA = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,1,0,0,0,
        0,0,1,0,0,0,
        0,0,1,1,0,0,
        0,1,0,1,0,0,
        0,1,0,1,0,0,
        0,1,1,1,1,0,
        0,1,0,0,1,0,
        1,1,0,0,1,1,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray iA = initBitArray(pA);

    QVector<bool> pB = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        1,1,1,1,0,0,
        0,1,0,0,1,0,
        0,1,0,0,1,0,
        0,1,1,1,0,0,
        0,1,0,0,1,0,
        0,1,0,0,1,0,
        0,1,0,0,1,0,
        1,1,1,1,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray iB = initBitArray(pB);

    QVector<bool> pC = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,1,1,1,1,0,
        1,0,0,0,1,0,
        1,0,0,0,0,0,
        1,0,0,0,0,0,
        1,0,0,0,0,0,
        1,0,0,0,0,0,
        1,0,0,0,1,0,
        0,1,1,1,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray iC = initBitArray(pC);

    QVector<bool> pF = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        1,1,1,1,1,0,
        0,1,0,0,1,0,
        0,1,0,1,0,0,
        0,1,1,1,0,0,
        0,1,0,1,0,0,
        0,1,0,0,0,0,
        0,1,0,0,0,0,
        1,1,1,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray iF = initBitArray(pF);

    QVector<bool> pH = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        1,1,0,0,1,1,
        0,1,0,0,1,0,
        0,1,0,0,1,0,
        0,1,1,1,1,0,
        0,1,0,0,1,0,
        0,1,0,0,1,0,
        0,1,0,0,1,0,
        1,1,0,0,1,1,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray iH = initBitArray(pH);

    QVector<bool> pI = {
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        1,1,1,1,1,0,
        0,0,1,0,0,0,
        0,0,1,0,0,0,
        0,0,1,0,0,0,
        0,0,1,0,0,0,
        0,0,1,0,0,0,
        0,0,1,0,0,0,
        1,1,1,1,1,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0};
    QBitArray iI = initBitArray(pI);

    QVector<bool> pw = {
        0,0,0,0,0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,1,1,1,0,
        0,0,0,1,0,0,0,0,0,0,0,0,
        0,0,0,1,0,0,0,0,0,0,0,0,
        0,0,0,1,0,0,0,0,0,0,0,0,
        0,0,0,1,1,1,1,1,1,1,0,0,
        0,0,0,1,0,0,0,0,0,1,0,0,
        0,0,0,1,0,0,0,0,0,1,0,0,
        0,0,1,0,0,0,0,0,0,1,0,0,
        0,0,1,0,0,0,0,0,0,1,0,0,
        0,1,0,0,0,0,0,0,0,1,0,0,
        1,0,0,0,0,0,1,1,1,0,0,0};
    QBitArray iw = initBitArray(pw);

    QVector<bool> py = {
        0,0,0,1,0,0,0,0,0,0,0,0,
        0,0,0,1,1,1,1,1,1,1,0,0,
        0,0,1,0,0,0,0,0,0,1,0,0,
        0,0,1,0,0,0,0,0,1,0,0,0,
        0,1,1,0,0,0,0,1,0,0,0,0,
        1,0,1,0,0,0,1,0,0,0,0,0,
        0,0,1,0,0,1,0,0,0,0,0,0,
        0,0,1,0,0,1,0,0,0,0,0,0,
        0,0,1,0,1,0,0,0,0,0,1,0,
        0,0,1,0,1,0,0,0,0,0,1,0,
        0,0,1,0,1,0,0,0,0,0,1,0,
        0,0,1,0,0,1,1,1,1,1,1,0};
    QBitArray iy = initBitArray(py);

    charPattern.insert(i0, '0');
    charPattern.insert(i1, '1');
    charPattern.insert(i2, '2');
    charPattern.insert(i3, '3');
    charPattern.insert(i4, '4');
    charPattern.insert(i5, '5');
    charPattern.insert(i6, '6');
    charPattern.insert(i7, '7');
    charPattern.insert(i8, '8');
    charPattern.insert(i9, '9');
    charPattern.insert(im, '-');
    charPattern.insert(ip, '.');
    charPattern.insert(ic, ':');
    charPattern.insert(is, '/');
    charPattern.insert(iA, 'A');
    charPattern.insert(iB, 'B');
    charPattern.insert(iC, 'C');
    charPattern.insert(iF, 'F');
    charPattern.insert(iH, 'H');
    charPattern.insert(iI, 'I');
    charPattern.insert(iw, 'w');
    charPattern.insert(iy, 'y');
}

bool Ocr::Scan(QPoint pos)
{
    cursorPos = pos;
    if(testChar(charSize))
    {
        seekingBackward();
        return decodeString();
    }
    else if(testChar(chineseSize))
    {
        for(int i=0; i<3; i++)
        {
            QPoint p = originPos;
            p.rx() -= i+charSize.width();
            if(decode(p, charSize))
            {
                originPos = p;
                seekingBackward();
                return decodeString();
            }
        }
    }
    return false;
}

QString Ocr::getString()
{
    return string;
}

QRect Ocr::getRect()
{
    return stringRect;
}

int Ocr::getScale()
{
    return scale;
}

QRgb Ocr::getColor()
{
    return color;
}

bool Ocr::testChar(const QSize &sz)
{
    QPoint pos(cursorPos);
    pos.rx() -= sz.width()-1;
    pos.ry() -= sz.height()-1;
    Rect area(pos, sz);

    char ch=0;

    while(area.next())
    {
        QRect padArea = area.translated(area.pos(false));
        if(!image.rect().contains(padArea, true))
            continue;
        auto samples = getSamples(padArea);
        if(samples.isEmpty())
            return false;
        if(testChar(samples))
        {
            ch=c;
            originPos = padArea.topLeft();
            if(c!='-')
                return true;
        }
    }
    if(ch)
        return true;
    return false;
}

void Ocr::seekingBackward()
{
    QPoint p(charSize.width(), 0);
    if(!image.rect().contains(originPos-p, true))
        return;
    while(decode(originPos-p, charSize))
    {
        originPos -= p;
        if(!image.rect().contains(originPos-p, true))
            return;
    }
}

bool Ocr::decodeString()
{
    string.clear();
    scale = 0;

    QPoint p = originPos;
    stringRect.setTopLeft(originPos);

    char last = 0;
    while(decode(p, charSize))
    {
        if(last=='-' && c=='-')
            return false;
        string.append(c);
        p.rx() += charSize.width();
        last = c;
    }
    QPoint p1 = p;
    p1.ry() += charSize.height();
    stringRect.setBottomRight(p1);

    for(int i=0; i<3; i++)
    {
        QPoint offset(i,0);
        if(decode(p+offset, chineseSize))
        {
            if(c=='w')
                scale = 10000;
            if(c=='y')
                scale = 100000000;
            p += offset;
            p.ry() += chineseSize.height();
            p.rx() += chineseSize.width();
            stringRect.setBottomRight(p);
            return true;
        }
    }
    if(string=="." ||
       string=="-" ||
       string==":" ||
       string=="/" ||
       string.count(".")>1 ||
       string.isEmpty())
        return false;
    return true;
}

bool Ocr::decode(const QPoint &pos, const QSize &sz)
{
    QRect rect(pos, sz);
    if(!image.rect().contains(rect, true))
        return false;
    auto sample = getSample(rect);
    if(!charPattern.contains(sample))
        return false;
    c = charPattern.value(sample);
    return true;
}

QHash<QRgb, QBitArray> Ocr::getSamples(const QRect &sampleArea)
{
    Rect area(sampleArea);
    QHash<QRgb, QBitArray> result;

    while(area.next())
    {
        QRgb c = image.pixel(area.pos());
        if(!result.contains(c))
        {
            QBitArray pad(area.count());
            result.insert(c, pad);
        }
        result[c].setBit(area.index());
    }
    return result;
}

QBitArray Ocr::getSample(const QRect &sampleArea)
{
    Rect area(sampleArea);
    QBitArray result(area.count());

    while(area.next())
    {
        if(image.pixel(area.pos()) == color)
            result.setBit(area.index());
    }
    return result;
}

bool Ocr::testChar(const QHash<QRgb, QBitArray> &samples)
{
    for(auto i = samples.begin(); i != samples.end(); ++i)
    {
        if(charPattern.contains(i.value()))
        {
            c = charPattern.value(i.value());
            color = i.key();
            return true;
        }
    }
    return false;
}

QBitArray Ocr::initBitArray(const QVector<bool> &pattern)
{
    QBitArray result(pattern.size());
    for(int i=0; i<pattern.size(); i++)
        if(pattern.at(i))
            result.setBit(i);
    return result;
}

