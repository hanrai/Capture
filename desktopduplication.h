#ifndef DESKTOPDUPLICATION_H
#define DESKTOPDUPLICATION_H

#include <QObject>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <client.h>
#include <QVector>
#include <QImage>
using Microsoft::WRL::ComPtr;

class DesktopDuplication : public QObject
{
    Q_OBJECT
public:
    explicit DesktopDuplication(QObject *parent = nullptr);
    void Init();
    bool isInitialized() {return m_initialized;}
    void takeSnapshots();
    QVector<QImage> getSnapshots() {return m_snapshots;}
    void Reset();

signals:

public slots:
private:
    bool m_initialized;
    QVector<ComPtr<ID3D11DeviceContext>>    m_contexts;
    QVector<ComPtr<IDXGIOutput1>>           m_outputs;
    QVector<ComPtr<ID3D11Device>>           m_devices;
    QVector<ComPtr<IDXGIOutputDuplication>> m_duplications;
    QVector<QImage>                         m_snapshots;
    QVector<ComPtr<ID3D11Texture2D>>        m_textures;
};

#endif // DESKTOPDUPLICATION_H
