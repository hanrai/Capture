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
bool ProcessFailure(_In_opt_ ID3D11Device* Device, _In_ LPCWSTR Str, _In_ LPCWSTR Title, HRESULT hr, _In_opt_z_ HRESULT* ExpectedErrors = nullptr);

void DisplayMsg(_In_ LPCWSTR Str, _In_ LPCWSTR Title, HRESULT hr);

struct _snapshotInfo
{
    QRgb* buffer;
    int width;
    int height;
} SnapshotInfo;

class DesktopDuplication : public QObject
{
    Q_OBJECT
public:
    explicit DesktopDuplication(QObject *parent = nullptr);
    int getOutputCount() {return m_outputs.size();}
    int getOutputID() {return m_outputID;}
    bool setOutputID(int id);
    QImage takeSnapshot();
    bool takeSnapshot(unsigned int output, SnapshotInfo *info);

public:
    class DuplicationException
    {
    public:
        DuplicationException(HRESULT rh, wchar_t *msg, wchar_t *file, int line):
            m_msg(msg), m_file(file), m_line(line), m_result(rh)
        {;}
    public:
        wchar_t *m_msg;
        wchar_t *m_file;
        int m_line;
        HRESULT m_result;
    };

signals:

public slots:
private:
    void initialize();
    void reset();
    DUPL_RETURN begin();
    bool prepareOutputMap();
    DUPL_RETURN trySanpshot(unsigned int output);
private:
    QVector<int>                            m_outputMap;
    bool                                    m_initialized;
    QVector<ComPtr<IDXGIOutput1>>           m_outputs;
    int                                     m_outputID;
    ComPtr<ID3D11Device>                    m_device;
    ComPtr<ID3D11DeviceContext>             m_context;
    ComPtr<IDXGIOutputDuplication>          m_duplication;
    ComPtr<ID3D11Texture2D>                 m_stagingTexture;
    QImage                                  m_snapshot;
    DYNAMIC_WAIT                            m_wait;
};

#endif // DESKTOPDUPLICATION_H
