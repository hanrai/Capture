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
    void Reset();
    int getOutputCount() {return m_outputs.size();}
    int getOutputID() {return m_outputID;}
    bool setOutputID(int id);
    QImage takeSnapshot();

signals:

public slots:
private:
    bool                                    m_initialized;
    QVector<ComPtr<IDXGIOutput1>>           m_outputs;
    int                                     m_outputID;
    ComPtr<ID3D11Device>                    m_device;
    ComPtr<ID3D11DeviceContext>             m_context;
    ComPtr<IDXGIOutputDuplication>          m_duplication;
    ComPtr<ID3D11Texture2D>                 m_stagingTexture;
    QImage                                  m_snapshot;
};

#endif // DESKTOPDUPLICATION_H
