#ifndef DESKTOPDUPLICATION_H
#define DESKTOPDUPLICATION_H

#include <QObject>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <client.h>
#include <QVector>
#include <QImage>
#include "dynamic_wait.h"

using Microsoft::WRL::ComPtr;


extern HRESULT SystemTransitionsExpectedErrors[];
extern HRESULT CreateDuplicationExpectedErrors[];
extern HRESULT FrameInfoExpectedErrors[];
extern HRESULT AcquireFrameExpectedError[];
extern HRESULT EnumOutputsExpectedErrors[];

typedef _Return_type_success_(return == DUPL_RETURN_SUCCESS) enum
{
    DUPL_RETURN_SUCCESS             = 0,
    DUPL_RETURN_ERROR_EXPECTED      = 1,
    DUPL_RETURN_ERROR_UNEXPECTED    = 2
}DUPL_RETURN;

_Post_satisfies_(return != DUPL_RETURN_SUCCESS)

void DisplayMsg(_In_ LPCWSTR Str, _In_ LPCWSTR Title, HRESULT hr);

struct SnapshotInfo
{
    QRgb* buffer;
    unsigned int width;
    unsigned int height;
    unsigned int pitch;
};

struct MetaData
{
    unsigned int size;
    unsigned int moveCount;
    unsigned int dirtyCount;
    BYTE *data;
    DXGI_OUTDUPL_MOVE_RECT *pMove;
    RECT *pDirty;
};

class DesktopDuplication : public QObject
{
    Q_OBJECT
public:
    explicit DesktopDuplication(QObject *parent = nullptr);
    int getOutputCount();
    QImage takeSnapshot(unsigned int output);
    bool beginCapture(unsigned int output);
    bool endCapture();
    bool capture(SnapshotInfo &info);

public:
    class Exception
    {
    public:
        LPCWSTR m_msg;
        const char *m_file;
        int m_line;
        HRESULT m_result;
    };

signals:

public slots:
private:
    bool trySanpshot(unsigned int output, QImage &snapShot);
    bool prepareOutputMap();
    bool begin(unsigned int output,
               ComPtr<ID3D11Device> &pDevice,
               ComPtr<ID3D11DeviceContext> &pContext,
               ComPtr<ID3D11Texture2D> &pStaging,
               ComPtr<IDXGIOutputDuplication> &pDuplication,
               DXGI_OUTPUT_DESC &outputDesc);
    bool tryCapture(ComPtr<ID3D11Device> &pDevice, ComPtr<ID3D11DeviceContext> &pContext,
                    ComPtr<ID3D11Texture2D> &pStaging, ComPtr<ID3D11Texture2D> &pTemp,
                    ComPtr<IDXGIOutputDuplication> &pDuplication, DXGI_OUTPUT_DESC &outputDesc,
                    MetaData &data, D3D11_MAPPED_SUBRESOURCE& mapInfo);
    void end(ComPtr<ID3D11Device> &pDevice,
             ComPtr<ID3D11DeviceContext> &pContext,
             ComPtr<ID3D11Texture2D> &pStaging,
             ComPtr<ID3D11Texture2D> &pTemp,
             ComPtr<IDXGIOutputDuplication> &pDuplication,
             DXGI_OUTPUT_DESC &outputDesc,
             D3D11_MAPPED_SUBRESOURCE &mapInfo,
             MetaData &data);

    void ProcessFailure(const char *file, int line, _In_opt_ ID3D11Device* Device, _In_ LPCWSTR Str,
                        _In_ LPCWSTR Title, HRESULT hr, _In_opt_z_ HRESULT* ExpectedErrors = nullptr);

    bool getFactory(ComPtr<IDXGIFactory1> &pFactory);
    bool getAdapter(ComPtr<IDXGIFactory1> &pFactory, ComPtr<IDXGIAdapter> &pAdapter, UINT output);
    bool getOutput(ComPtr<IDXGIAdapter> &pAdapter, ComPtr<IDXGIOutput1> &pOutput, UINT output);
    bool getD3DSets(ComPtr<IDXGIAdapter> &pAdapter, ComPtr<ID3D11Device> &pDevice,
                    ComPtr<ID3D11DeviceContext> &pContext);
    bool getDuplication(ComPtr<IDXGIOutput1> &pOutput, ComPtr<ID3D11Device> &pDevice,
                        ComPtr<IDXGIOutputDuplication> &pDuplication);
    bool getStaging(ComPtr<IDXGIOutput1> &pOutput, ComPtr<ID3D11Device> &pDevice,
                    ComPtr<ID3D11Texture2D> &pStaging, DXGI_OUTPUT_DESC &outputDesc);
    bool releaseFrame(ComPtr<IDXGIOutputDuplication> &pDuplication);
    bool getFrame(ComPtr<IDXGIOutputDuplication> &pDuplication, DXGI_OUTDUPL_FRAME_INFO &info,
                  ComPtr<ID3D11Texture2D> &pFrame);
    bool getMetaData(unsigned int size, MetaData &data);
    bool getChanges(ComPtr<IDXGIOutputDuplication> &pDuplication, MetaData &data);
    bool copyMove(ComPtr<ID3D11DeviceContext> &pContext, ComPtr<ID3D11Device> &pDevice,
                  ComPtr<ID3D11Texture2D> &pStaging, ComPtr<ID3D11Texture2D> &pTemp, MetaData &data,
                  DXGI_OUTPUT_DESC &outputDesc);
    bool copyDirty(ComPtr<ID3D11DeviceContext> &pContext, MetaData &data,
                   ComPtr<ID3D11Texture2D> &pStaging, ComPtr<ID3D11Texture2D> &pFrame);
    bool getMapInfo(ComPtr<ID3D11DeviceContext> &pContext, ComPtr<ID3D11Texture2D> &pStaging,
                    D3D11_MAPPED_SUBRESOURCE &mapInfo);
private:
    QVector<UINT>                           m_outputMap;
    DYNAMIC_WAIT                            m_wait;
    ComPtr<ID3D11Device> m_pDevice;
    ComPtr<ID3D11DeviceContext> m_pContext;
    ComPtr<ID3D11Texture2D> m_pStaging;
    ComPtr<ID3D11Texture2D> m_pTemp;
    ComPtr<IDXGIOutputDuplication> m_pDuplication;
    DXGI_OUTPUT_DESC m_outputDesc;
    D3D11_MAPPED_SUBRESOURCE m_mapInfo;
    MetaData m_data;
    bool m_running;
};

#endif // DESKTOPDUPLICATION_H
