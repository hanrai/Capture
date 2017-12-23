#include <windows.h>
#include "desktopduplication.h"
#include <QtDebug>
#include <dxgi1_6.h>
#include <stdexcept>
#include <QCoreApplication>

// These are the errors we expect from general Dxgi API due to a transition
HRESULT SystemTransitionsExpectedErrors[] = {
                                                DXGI_ERROR_DEVICE_REMOVED,
                                                DXGI_ERROR_ACCESS_LOST,
                                                static_cast<HRESULT>(WAIT_ABANDONED),
                                                S_OK                                    // Terminate list with zero valued HRESULT
                                            };

// These are the errors we expect from IDXGIOutput1::DuplicateOutput due to a transition
HRESULT CreateDuplicationExpectedErrors[] = {
                                                DXGI_ERROR_DEVICE_REMOVED,
                                                static_cast<HRESULT>(E_ACCESSDENIED),
                                                DXGI_ERROR_UNSUPPORTED,
                                                DXGI_ERROR_SESSION_DISCONNECTED,
                                                S_OK                                    // Terminate list with zero valued HRESULT
                                            };

// These are the errors we expect from IDXGIOutputDuplication methods due to a transition
HRESULT FrameInfoExpectedErrors[] = {
                                        DXGI_ERROR_DEVICE_REMOVED,
                                        DXGI_ERROR_ACCESS_LOST,
                                        S_OK                                    // Terminate list with zero valued HRESULT
                                    };

// These are the errors we expect from IDXGIAdapter::EnumOutputs methods due to outputs becoming stale during a transition
HRESULT EnumOutputsExpectedErrors[] = {
                                          DXGI_ERROR_NOT_FOUND,
                                          S_OK                                    // Terminate list with zero valued HRESULT
                                      };

void DesktopDuplication::ProcessFailure(const char* file, int line, _In_opt_ ID3D11Device* Device, _In_ LPCWSTR Str, _In_ LPCWSTR Title, HRESULT hr, _In_opt_z_ HRESULT* ExpectedErrors)
{
    HRESULT TranslatedHr;

    // On an error check if the DX device is lost
    if (Device)
    {
        HRESULT DeviceRemovedReason = Device->GetDeviceRemovedReason();

        switch (DeviceRemovedReason)
        {
            case DXGI_ERROR_DEVICE_REMOVED :
            case DXGI_ERROR_DEVICE_RESET :
            case static_cast<HRESULT>(E_OUTOFMEMORY) :
            {
                // Our device has been stopped due to an external event on the GPU so map them all to
                // device removed and continue processing the condition
                TranslatedHr = DXGI_ERROR_DEVICE_REMOVED;
                break;
            }

            case S_OK :
            {
                // Device is not removed so use original error
                TranslatedHr = hr;
                break;
            }

            default :
            {
                // Device is removed but not a error we want to remap
                TranslatedHr = DeviceRemovedReason;
            }
        }
    }
    else
    {
        TranslatedHr = hr;
    }

    // Check if this error was expected or not
    if (ExpectedErrors)
    {
        HRESULT* CurrentResult = ExpectedErrors;

        while (*CurrentResult != S_OK)
        {
            if (*(CurrentResult++) == TranslatedHr)
            {
                return;
            }
        }
    }

    Exception exception;
    exception.m_msg = Str;
    exception.m_file = file;
    exception.m_line = line;
    exception.m_result = TranslatedHr;
    throw exception;
    return; //never run;
}

bool DesktopDuplication::getFactory(ComPtr<IDXGIFactory1> &pFactory)
{
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
    if (FAILED(hr))
    {
        ProcessFailure(__FILE__, __LINE__, nullptr, L"Failed to get DXGI Factory", L"Error", hr, SystemTransitionsExpectedErrors);
        return false;
    }
    return true;
}

bool DesktopDuplication::getAdapter(ComPtr<IDXGIFactory1> &pFactory,
                                    ComPtr<IDXGIAdapter> &pAdapter, UINT output)
{
    UINT adapterIndex = m_outputMap.at(output);
    HRESULT hr = pFactory->EnumAdapters(adapterIndex, &pAdapter);
    if (FAILED(hr))
    {
        if(hr == DXGI_ERROR_NOT_FOUND)
        {
            return false;
        }
        ProcessFailure(__FILE__, __LINE__, nullptr, L"Failed to get DXGI Factory", L"Error", hr, SystemTransitionsExpectedErrors);
        return false;
    }
    return true;
}

bool DesktopDuplication::getOutput(ComPtr<IDXGIAdapter> &pAdapter,
                                   ComPtr<IDXGIOutput1> &pOutput, UINT output)
{
    UINT outputIndex = output - m_outputMap.indexOf(m_outputMap.at(output)); //Little tricky, not clear enough.
    ComPtr<IDXGIOutput> pOut;
    HRESULT hr = pAdapter->EnumOutputs(outputIndex, &pOut);
    if (FAILED(hr))
    {
        ProcessFailure(__FILE__, __LINE__, nullptr, L"Output specified to be duplicated does not exist", L"Error", hr);
        return false;
    }
    hr = pOut.As(&pOutput);
    if (FAILED(hr))
    {
        ProcessFailure(__FILE__, __LINE__, nullptr, L"Failed to QI for DxgiOutput1 in DUPLICATIONMANAGER", L"Error", hr);
        return false;
    }
    return true;
}

bool DesktopDuplication::getD3DSets(ComPtr<IDXGIAdapter> &pAdapter, ComPtr<ID3D11Device> &pDevice, ComPtr<ID3D11DeviceContext> &pContext)
{
    D3D_FEATURE_LEVEL FeatureLevels[] = {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_1};
    HRESULT hr = D3D11CreateDevice(
                pAdapter.Get(),
                D3D_DRIVER_TYPE_UNKNOWN,
                nullptr,
                /*D3D11_CREATE_DEVICE_DEBUG*/0,
                FeatureLevels,
                ARRAYSIZE(FeatureLevels),
                D3D11_SDK_VERSION,
                &pDevice,
                nullptr,
                &pContext);
    if (FAILED(hr))
    {
        ProcessFailure(__FILE__, __LINE__, pDevice.Get(), L"Device creation failed", L"Error", hr, SystemTransitionsExpectedErrors);
        return false;
    }
    return true;
}

bool DesktopDuplication::getDuplication(ComPtr<IDXGIOutput1> &pOutput, ComPtr<ID3D11Device> &pDevice, ComPtr<IDXGIOutputDuplication> &pDuplication)
{
    HRESULT hr = pOutput->DuplicateOutput(pDevice.Get(), &pDuplication);
    if (FAILED(hr))
    {
        if (hr == DXGI_ERROR_NOT_CURRENTLY_AVAILABLE)
        {
            ProcessFailure(__FILE__, __LINE__, pDevice.Get(), L"There is already the maximum number of applications using the Desktop Duplication API running, please close one of those applications and then try again.", L"Error", hr, CreateDuplicationExpectedErrors);
            return false;
        }
        ProcessFailure(__FILE__, __LINE__, pDevice.Get(), L"Failed to get duplicate output", L"Error", hr, CreateDuplicationExpectedErrors);
        return false;
    }
    return true;
}

bool DesktopDuplication::getStaging(ComPtr<IDXGIOutput1> &pOutput, ComPtr<ID3D11Device> &pDevice, ComPtr<ID3D11Texture2D> &pStaging, DXGI_OUTPUT_DESC &outputDesc)
{
    RtlZeroMemory(&outputDesc, sizeof(outputDesc));
    pOutput->GetDesc(&outputDesc);

    D3D11_TEXTURE2D_DESC stagingDescriptor;
    ZeroMemory(&stagingDescriptor, sizeof(stagingDescriptor));
    stagingDescriptor.Width = outputDesc.DesktopCoordinates.right - outputDesc.DesktopCoordinates.left;
    stagingDescriptor.Height = outputDesc.DesktopCoordinates.bottom - outputDesc.DesktopCoordinates.top;
    stagingDescriptor.Usage = D3D11_USAGE_STAGING;
    stagingDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    stagingDescriptor.BindFlags = 0;
    stagingDescriptor.MiscFlags = 0;
    stagingDescriptor.MipLevels = 1;
    stagingDescriptor.ArraySize = 1;
    stagingDescriptor.SampleDesc.Count = 1;
    stagingDescriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

    HRESULT hr = pDevice->CreateTexture2D(&stagingDescriptor, NULL, &pStaging);
    if (FAILED(hr))
    {
        ProcessFailure(__FILE__, __LINE__, pDevice.Get(), L"Failed to create staging texture", L"Error", hr, SystemTransitionsExpectedErrors);
        return false;
    }
    return true;
}

bool DesktopDuplication::tryCapture(ComPtr<ID3D11Device> &pDevice, ComPtr<ID3D11DeviceContext> &pContext, ComPtr<ID3D11Texture2D> &pStaging, ComPtr<ID3D11Texture2D> &pTemp, ComPtr<IDXGIOutputDuplication> &pDuplication, DXGI_OUTPUT_DESC &outputDesc, MetaData &data, D3D11_MAPPED_SUBRESOURCE& mapInfo)
{
    if(!releaseFrame(pDuplication))
        return false;

    pContext->Unmap(pStaging.Get(), 0);

    DXGI_OUTDUPL_FRAME_INFO info;
    ComPtr<ID3D11Texture2D> pFrame;
    if(!getFrame(pDuplication, info, pFrame))
        return false;

    if(!getMetaData(info.TotalMetadataBufferSize, data))
        return false;

    if(!getChanges(pDuplication, data))
        return false;

    if(!copyMove(pContext, pDevice, pStaging, pTemp, data, outputDesc))
        return false;

    if(!copyDirty(pContext, data, pStaging, pFrame))
        return false;

    if(!getMapInfo(pContext, pStaging, mapInfo))
        return false;

    return true;
}

bool DesktopDuplication::releaseFrame(ComPtr<IDXGIOutputDuplication> &pDuplication)
{
    HRESULT hr = pDuplication->ReleaseFrame();
    if (FAILED(hr) && hr != DXGI_ERROR_INVALID_CALL)
    {
        ProcessFailure(__FILE__, __LINE__, nullptr, L"Failed to release frame in DUPLICATIONMANAGER", L"Error", hr, FrameInfoExpectedErrors);
        return false;
    }
    return true;
}

bool DesktopDuplication::getFrame(ComPtr<IDXGIOutputDuplication> &pDuplication, DXGI_OUTDUPL_FRAME_INFO &info, ComPtr<ID3D11Texture2D> &pFrame)
{
    HRESULT hr;
    ComPtr<IDXGIResource> resource;
    forever
    {
        hr = pDuplication->AcquireNextFrame(500, &info, &resource);
        if(hr==DXGI_ERROR_WAIT_TIMEOUT || info.TotalMetadataBufferSize==0)
        {
            releaseFrame(pDuplication);
            continue;
        }
        if (FAILED(hr))
        {
            ProcessFailure(__FILE__, __LINE__, nullptr, L"Failed to acquire next frame in DUPLICATIONMANAGER", L"Error", hr, FrameInfoExpectedErrors);
            return false;
        }
        break;
    }

    hr = resource.As(&pFrame);
    if (FAILED(hr))
    {
        ProcessFailure(__FILE__, __LINE__, nullptr, L"Failed to QI for ID3D11Texture2D from acquired IDXGIResource in DUPLICATIONMANAGER", L"Error", hr);
        return false;
    }
    return true;
}

bool DesktopDuplication::getMetaData(unsigned int size, MetaData &data)
{
    if (size > data.size)
    {
        if (data.data)
        {
            delete [] data.data;
            data.data = nullptr;
        }
        data.data = new (std::nothrow) BYTE[size];
        if (!data.data)
        {
            RtlZeroMemory(&data, sizeof(data));
            ProcessFailure(__FILE__, __LINE__, nullptr, L"Failed to allocate memory for metadata in DUPLICATIONMANAGER", L"Error", E_OUTOFMEMORY);
            return false;
        }
        data.size = size;
    }

    return true;
}

bool DesktopDuplication::getChanges(ComPtr<IDXGIOutputDuplication> &pDuplication, MetaData &data)
{
    UINT BufSize = data.size;

    // Get move rectangles
    data.pMove = (DXGI_OUTDUPL_MOVE_RECT*)data.data;
    HRESULT hr = pDuplication->GetFrameMoveRects(BufSize, data.pMove, &BufSize);
    if (FAILED(hr))
    {
        data.dirtyCount = 0;
        data.moveCount = 0;
        ProcessFailure(__FILE__, __LINE__, nullptr, L"Failed to get frame move rects in DUPLICATIONMANAGER", L"Error", hr, FrameInfoExpectedErrors);
        return false;
    }
    data.moveCount = BufSize / sizeof(DXGI_OUTDUPL_MOVE_RECT);

    data.pDirty = (RECT*)(data.data + BufSize);
    BufSize = data.size - BufSize;

    // Get dirty rectangles
    hr = pDuplication->GetFrameDirtyRects(BufSize, data.pDirty, &BufSize);
    if (FAILED(hr))
    {
        data.dirtyCount = 0;
        data.moveCount = 0;
        ProcessFailure(__FILE__, __LINE__, nullptr, L"Failed to get frame dirty rects in DUPLICATIONMANAGER", L"Error", hr, FrameInfoExpectedErrors);
        return false;
    }
    data.dirtyCount = BufSize / sizeof(RECT);

    return true;
}

bool DesktopDuplication::copyMove(ComPtr<ID3D11DeviceContext> &pContext, ComPtr<ID3D11Device> &pDevice, ComPtr<ID3D11Texture2D> &pStaging, ComPtr<ID3D11Texture2D> &pTemp, MetaData &data, DXGI_OUTPUT_DESC &outputDesc)
{
    if(!data.moveCount)
        return true;

    D3D11_TEXTURE2D_DESC stagingDesc;
    pStaging->GetDesc(&stagingDesc);

    // Make new intermediate surface to copy into for moving
    if (!pTemp)
    {
        D3D11_TEXTURE2D_DESC tempDesc;
        tempDesc = stagingDesc;
        tempDesc.Width = outputDesc.DesktopCoordinates.right - outputDesc.DesktopCoordinates.left;
        tempDesc.Height = outputDesc.DesktopCoordinates.bottom - outputDesc.DesktopCoordinates.top;
        tempDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
        tempDesc.MiscFlags = 0;
        HRESULT hr = pDevice->CreateTexture2D(&tempDesc, nullptr, &pTemp);
        if (FAILED(hr))
        {
            ProcessFailure(__FILE__, __LINE__, pDevice.Get(), L"Failed to create staging texture for move rects", L"Error", hr, SystemTransitionsExpectedErrors);
            return false;
        }
    }

    for (UINT i = 0; i < data.moveCount; ++i)
    {
        // Copy rect out of shared surface
        D3D11_BOX Box;
        Box.left = data.pMove[i].SourcePoint.x;
        Box.top = data.pMove[i].SourcePoint.y;
        Box.front = 0;
        Box.right = Box.left + data.pMove[i].DestinationRect.right - data.pMove[i].DestinationRect.left;
        Box.bottom = Box.top + data.pMove[i].DestinationRect.bottom - data.pMove[i].DestinationRect.top;
        Box.back = 1;
        pContext->CopySubresourceRegion(pTemp.Get(), 0, 0, 0, 0, pStaging.Get(), 0, &Box);

        // Copy back to shared surface
        Box.left = 0;
        Box.top = 0;
        Box.front = 0;
        Box.right = data.pMove[i].DestinationRect.right - data.pMove[i].DestinationRect.left;
        Box.bottom = data.pMove[i].DestinationRect.bottom - data.pMove[i].DestinationRect.top;
        Box.back = 1;
        pContext->CopySubresourceRegion(pStaging.Get(), 0,
                                               data.pMove[i].DestinationRect.left,
                                               data.pMove[i].DestinationRect.top,
                                               0, pTemp.Get(), 0, &Box);
    }
    return true;
}

bool DesktopDuplication::copyDirty(ComPtr<ID3D11DeviceContext> &pContext, MetaData &data, ComPtr<ID3D11Texture2D> &pStaging, ComPtr<ID3D11Texture2D> &pFrame)
{
    if(!data.dirtyCount)
        return true;

    for (UINT i = 0; i < data.dirtyCount; ++i)
    {
        // Copy rect out of shared surface
        D3D11_BOX Box;
        Box.left = data.pDirty[i].left;
        Box.top = data.pDirty[i].top;
        Box.front = 0;
        Box.right = data.pDirty[i].right;
        Box.bottom = data.pDirty[i].bottom;
        Box.back = 1;
        pContext->CopySubresourceRegion(pStaging.Get(), 0, Box.left, Box.top, 0, pFrame.Get(), 0, &Box);
    }
    return true;
}

bool DesktopDuplication::getMapInfo(ComPtr<ID3D11DeviceContext> &pContext, ComPtr<ID3D11Texture2D> &pStaging, D3D11_MAPPED_SUBRESOURCE &mapInfo)
{
    HRESULT hr = pContext->Map(
                pStaging.Get(),
                0,  // Subresource
                D3D11_MAP_READ,
                0,  // MapFlags
                &mapInfo);
    if (FAILED(hr))
    {
        ProcessFailure(__FILE__, __LINE__, nullptr, L"Failed to map surface for pointer", L"Error", hr, SystemTransitionsExpectedErrors);
        return false;
    }
    return true;
}

DesktopDuplication::DesktopDuplication(QObject *parent) :
    QObject(parent)
{
    ZeroMemory(&m_outputDesc, sizeof(m_outputDesc));
    ZeroMemory(&m_mapInfo, sizeof(m_mapInfo));
    ZeroMemory(&m_data, sizeof(m_data));
    m_running = false;
}

int DesktopDuplication::getOutputCount()
{
    if(m_outputMap.isEmpty())
        prepareOutputMap();
    return m_outputMap.size();
}

bool DesktopDuplication::begin(unsigned int output,
                               ComPtr<ID3D11Device> &pDevice,
                               ComPtr<ID3D11DeviceContext> &pContext,
                               ComPtr<ID3D11Texture2D> &pStaging,
                               ComPtr<IDXGIOutputDuplication> &pDuplication,
                               DXGI_OUTPUT_DESC &outputDesc)
{
    if(output >= m_outputMap.size())
    {
        if(!prepareOutputMap())
            return false;
    }

    if(output >= m_outputMap.size())
    {
        ProcessFailure(__FILE__, __LINE__, nullptr, L"Output not found.", L"Error", DXGI_ERROR_NOT_FOUND);
        return false;
    }

    ComPtr<IDXGIFactory1> pFactory;
    if(!getFactory(pFactory))
        return false;

    ComPtr<IDXGIAdapter> pAdapter;
    if(!getAdapter(pFactory, pAdapter, output))
        return false;

    ComPtr<IDXGIOutput1> pOutput;
    if(!getOutput(pAdapter, pOutput, output))
        return false;

    if(!getD3DSets(pAdapter, pDevice, pContext))
        return false;

    if(!getDuplication(pOutput, pDevice, pDuplication))
        return false;

    if(!getStaging(pOutput, pDevice, pStaging, outputDesc))
        return false;

    return true;
}

void DesktopDuplication::end(ComPtr<ID3D11Device> &pDevice, ComPtr<ID3D11DeviceContext> &pContext, ComPtr<ID3D11Texture2D> &pStaging, ComPtr<ID3D11Texture2D> &pTemp, ComPtr<IDXGIOutputDuplication> &pDuplication, DXGI_OUTPUT_DESC &outputDesc, D3D11_MAPPED_SUBRESOURCE &mapInfo, MetaData &data)
{
    releaseFrame(pDuplication);
    pContext->Unmap(pStaging.Get(), 0);
    if (data.data)
    {
        delete [] data.data;
        data.data = nullptr;
    }
    RtlZeroMemory(&outputDesc, sizeof(outputDesc));
    RtlZeroMemory(&mapInfo, sizeof(mapInfo));
    RtlZeroMemory(&data, sizeof(data));
    pDevice.Reset();
    pContext.Reset();
    pStaging.Reset();
    pTemp.Reset();
    pDuplication.Reset();
}

bool DesktopDuplication::prepareOutputMap()
{
    m_outputMap.clear();

    ComPtr<IDXGIFactory1> pFactory;
    if(!getFactory(pFactory))
        return false;

    HRESULT hr = S_OK;

    for(int adapterCount = 0; SUCCEEDED(hr); ++adapterCount)
    {
        ComPtr<IDXGIAdapter> pAdapter;
        hr = pFactory->EnumAdapters(adapterCount, &pAdapter);
        if(pAdapter && (hr != DXGI_ERROR_NOT_FOUND))
        {
            HRESULT ho = S_OK;
            for(int outputCount = 0; SUCCEEDED(ho); ++outputCount)
            {
                ComPtr<IDXGIOutput> pOutput;
                ho = pAdapter->EnumOutputs(outputCount, &pOutput);
                if(pOutput && (ho != DXGI_ERROR_NOT_FOUND))
                {
                    m_outputMap.append(adapterCount);
                }
            }
        }
    }

    if(m_outputMap.isEmpty())
    {
        return false;
    }

    return true;
}

bool DesktopDuplication::trySanpshot(unsigned int output, QImage &snapShot)
{
    ComPtr<ID3D11Device> pDevice;
    ComPtr<ID3D11DeviceContext> pContext;
    ComPtr<ID3D11Texture2D> pStaging;
    ComPtr<ID3D11Texture2D> pTemp;
    ComPtr<IDXGIOutputDuplication> pDuplication;
    DXGI_OUTPUT_DESC outputDesc;
    D3D11_MAPPED_SUBRESOURCE mapInfo;
    MetaData data;
    RtlZeroMemory(&outputDesc, sizeof(outputDesc));
    RtlZeroMemory(&mapInfo, sizeof(mapInfo));
    RtlZeroMemory(&data, sizeof(data));


    if(!begin(output, pDevice, pContext, pStaging, pDuplication, outputDesc))
        return false;

    if(!tryCapture(pDevice, pContext, pStaging, pTemp, pDuplication, outputDesc, data, mapInfo))
        return false;

    auto height = outputDesc.DesktopCoordinates.bottom - outputDesc.DesktopCoordinates.top;
    auto width = outputDesc.DesktopCoordinates.right - outputDesc.DesktopCoordinates.left;
    QImage img(width, height, QImage::Format_ARGB32);
    for(int i=0; i<height; i++)
    {
        memcpy(
            (void*)((char*)img.bits()+i*img.width()*4),
            (void*)((char*)mapInfo.pData+i*mapInfo.RowPitch),
            img.width()*4);
    }
    snapShot = img;

    end(pDevice, pContext, pStaging, pTemp, pDuplication, outputDesc, mapInfo, data);

    return true;
}

QImage DesktopDuplication::takeSnapshot(unsigned int output)
{
    QImage snapShot;
    forever
    {
        if(!trySanpshot(output, snapShot))
        {
            m_wait.Wait();
            continue;
        }
        break;
    }
    return snapShot;
}

bool DesktopDuplication::beginCapture(unsigned int output)
{
    if(m_running)
        end(m_pDevice, m_pContext, m_pStaging, m_pTemp, m_pDuplication, m_outputDesc, m_mapInfo, m_data);

    m_running = false;

    if(!begin(output, m_pDevice, m_pContext, m_pStaging, m_pDuplication, m_outputDesc))
        return false;

    m_running = true;
    return true;
}

bool DesktopDuplication::endCapture()
{
    if(!m_running)
        return true;
    end(m_pDevice, m_pContext, m_pStaging, m_pTemp, m_pDuplication, m_outputDesc, m_mapInfo, m_data);
    m_running = false;
    return true;
}

bool DesktopDuplication::capture(SnapshotInfo &info)
{
    if(!m_running)
        return false;

    if(!tryCapture(m_pDevice, m_pContext, m_pStaging, m_pTemp, m_pDuplication, m_outputDesc, m_data, m_mapInfo))
        return false;

    info.height = m_outputDesc.DesktopCoordinates.bottom - m_outputDesc.DesktopCoordinates.top;
    info.width = m_outputDesc.DesktopCoordinates.right - m_outputDesc.DesktopCoordinates.left;
    info.buffer = (QRgb*)m_mapInfo.pData;
    info.pitch = m_mapInfo.RowPitch;
    return true;
}
