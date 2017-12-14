#include <windows.h>
#include "desktopduplication.h"
#include <QtDebug>
#include <dxgi1_6.h>
#include <stdexcept>

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

//
// Displays a message
//
void DisplayMsg(_In_ LPCWSTR Str, _In_ LPCWSTR Title, HRESULT hr)
{
    if (SUCCEEDED(hr))
    {
        MessageBoxW(nullptr, Str, Title, MB_OK);
        return;
    }

    const UINT StringLen = (UINT)(wcslen(Str) + sizeof(" with HRESULT 0x########."));
    wchar_t* OutStr = new wchar_t[StringLen];
    if (!OutStr)
    {
        return;
    }

    INT LenWritten = swprintf_s(OutStr, StringLen, L"%s with 0x%X.", Str, hr);
    if (LenWritten != -1)
    {
        MessageBoxW(nullptr, OutStr, Title, MB_OK);
    }

    delete [] OutStr;
}

bool ProcessFailure(_In_opt_ ID3D11Device* Device, _In_ LPCWSTR Str, _In_ LPCWSTR Title, HRESULT hr, _In_opt_z_ HRESULT* ExpectedErrors)
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
                return DUPL_RETURN_ERROR_EXPECTED;
            }
        }
    }

    // Error was not expected so display the message box
    DisplayMsg(Str, Title, TranslatedHr);

    return DUPL_RETURN_ERROR_UNEXPECTED;
}

DesktopDuplication::DesktopDuplication(QObject *parent) :
    QObject(parent),
    m_initialized(false)
{
    initialize();
}

void DesktopDuplication::initialize()
{
    if(m_initialized)
        reset();

    DUPL_RETURN dr = prepareOutputMap();
    //
    //Create a DXGI Factory first.
    //
    ComPtr<IDXGIFactory1> pFactory;
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
    if(SUCCEEDED(hr))
    {
        //
        //Enumeration adapters installed in this machine.
        //
        ComPtr<IDXGIAdapter> pAdapter;
        for(int i=0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; i++)
        {
            ComPtr<IDXGIOutput> pOutput;
            for(int i=0; pAdapter->EnumOutputs(i, &pOutput) != DXGI_ERROR_NOT_FOUND; i++)
            {
                ComPtr<IDXGIOutput1> pOutput1;
                hr = pOutput.As(&pOutput1);
                if(SUCCEEDED(hr))
                {
                    m_outputs.push_back(pOutput1);
                }
            }
        }
    }
    qDebug()<<"Outputs:"<<m_outputs.size();
    if(FAILED(hr))
    {
        qCritical()<<"Init error:"<<(void*)hr;
        return;
    }
    if(m_outputs.isEmpty())
    {
        qCritical()<<"No output found";
        return;
    }
    m_initialized = true;
}

void DesktopDuplication::reset()
{
    m_initialized = false;
    m_outputMap.clear();
    ///old part
    m_outputs.clear();
    m_outputID = -1;
    m_device.Reset();
    m_context.Reset();
    m_duplication.Reset();
    m_stagingTexture.Reset();
}

DUPL_RETURN DesktopDuplication::begin()
{
    DUPL_RETURN result = DUPL_RETURN_SUCCESS;
    if(m_outputMap.isEmpty())
    {
        result = prepareOutputMap();
    }
    return result;
}

bool DesktopDuplication::prepareOutputMap()
{
    m_outputMap.clear();

    ComPtr<IDXGIFactory> pFactory;
    HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(pFactory.GetAddressOf()));
    if (FAILED(hr))
    {
        return ProcessFailure(nullptr, L"Failed to get DXGI Factory", L"Error", hr, SystemTransitionsExpectedErrors);
    }

    for(int adapterCount = 0; SUCCEEDED(hr); ++adapterCount)
    {
        ComPtr<IDXGIAdapter> pAdapter;
        hr = pFactory->EnumAdapters(adapterCount, &pAdapter);
        if(pAdapter.Get() && (hr != DXGI_ERROR_NOT_FOUND))
        {
            HRESULT ho = S_OK;
            for(int outputCount = 0; SUCCEEDED(ho); ++outputCount)
            {
                ComPtr<IDXGIOutput> pOutput;
                ho = pAdapter->EnumOutputs(outputCount, &pOutput);
                if(pOutput.Get() && (ho != DXGI_ERROR_NOT_FOUND))
                {
                    m_outputMap.append(adapterCount);
                }
            }
        }
    }

    if(m_outputMap.isEmpty())
    {
        return DUPL_RETURN_ERROR_EXPECTED;
    }

    return DUPL_RETURN_SUCCESS;
}

DUPL_RETURN DesktopDuplication::trySanpshot(unsigned int output)
{
    if(output >= m_outputMap.size())
    {
        prepareOutputMap();
    }

    if(m_outputMap.isEmpty())
    {
        return DUPL_RETURN_ERROR_EXPECTED;
    }

    if(output >= m_outputMap.size())
        throw std::out_of_range(L"Output not available!");
    return DUPL_RETURN_SUCCESS;
}

bool DesktopDuplication::setOutputID(int id)
{
    if(!m_initialized)
        initialize();

    if(id<0 || id>=m_outputs.size())
    {
        qCritical()<<"Output id error:"<<id<<" of "<<m_outputs.size();
        return false;
    }

    HRESULT hr;

    auto output = m_outputs.at(id);
    ComPtr<IDXGIAdapter> pAdapter;

    hr = output->GetParent(IID_PPV_ARGS(&pAdapter));
    if(SUCCEEDED(hr))
    {
        m_device.Reset();
        m_context.Reset();
        D3D_FEATURE_LEVEL FeatureLevels[] = {
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0};
        hr = D3D11CreateDevice(
                    pAdapter.Get(),             //pAdapter [in, optional]
                    D3D_DRIVER_TYPE_UNKNOWN,    //DriverType
                    nullptr,                    //Software
                    /*D3D11_CREATE_DEVICE_DEBUG*/0,  //Flags
                    FeatureLevels,              //pFeatureLevels
                    ARRAYSIZE(FeatureLevels),   //FeatureLevels
                    D3D11_SDK_VERSION,          //SDKVersion
                    &m_device,           //ppDevice
                    nullptr,                    //pFeatureLevel
                    &m_context);         //context
        if(SUCCEEDED(hr))
        {
            m_duplication.Reset();
            hr = output->DuplicateOutput(m_device.Get(), &m_duplication);
            if(SUCCEEDED(hr))
            {
                DXGI_OUTPUT_DESC outputDesc;
                hr = output->GetDesc(&outputDesc);
                if(SUCCEEDED(hr))
                {
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

                    m_stagingTexture.Reset();
                    hr = m_device->CreateTexture2D(&stagingDescriptor, NULL, &m_stagingTexture);
                    if(SUCCEEDED(hr))
                    {
                        m_snapshot = QImage(stagingDescriptor.Width, stagingDescriptor.Height, QImage::Format_ARGB32);
                    }
                }
            }
        }
    }
    return SUCCEEDED(hr);
}

QImage DesktopDuplication::takeSnapshot()
{
    DXGI_OUTDUPL_FRAME_INFO info;
    ComPtr<IDXGIResource> resource;
    HRESULT hr = m_duplication->AcquireNextFrame(
                3000,                       //TimeoutInMilliseconds
                &info,                      //pFrameInfo
                &resource);                 //ppDesktopResource
    if(SUCCEEDED(hr))
    {
        //qDebug()<<info.AccumulatedFrames;
        ComPtr<ID3D11Texture2D> frameTexture;
        hr = resource.As(&frameTexture);
        if(SUCCEEDED(hr))
        {
            m_context->CopyResource(m_stagingTexture.Get(), frameTexture.Get());

            D3D11_MAPPED_SUBRESOURCE mapInfo;
            hr = m_context->Map(
                        m_stagingTexture.Get(),
                        0,  // Subresource
                        D3D11_MAP_READ,
                        0,  // MapFlags
                        &mapInfo);
            if(SUCCEEDED(hr))
            {
                //qDebug()<<"Texture mapped.";
                for(int i=0; i<m_snapshot.height(); i++)
                {
                    memcpy(
                        (void*)((char*)m_snapshot.bits()+i*m_snapshot.width()*4),
                        (void*)((char*)mapInfo.pData+i*mapInfo.RowPitch),
                        m_snapshot.width()*4
                          );
                }
            }
            m_context->Unmap(m_stagingTexture.Get(), 0);
        }
        hr = m_duplication->ReleaseFrame();
    }
    else
        qDebug()<<(void*)hr;

//qDebug()<<(void*)hr;
    return m_snapshot;
}

bool DesktopDuplication::takeSnapshot(unsigned int output, SnapshotInfo *info)
{
    DUPL_RETURN ret = DUPL_RETURN_SUCCESS;
    info->buffer = nullptr;
    forever
    {
        ret = trySnapshot(output);
        if(ret == DUPL_RETURN_ERROR_EXPECTED)
        {
            m_wait.Wait();
            continue;
        }
//        if(output >= m_outputMap.size())
//        {
//            ret = prepareOutputMap();
//        }
//        if(ret != DUPL_RETURN_SUCCESS)
//        {
//            if(ret == DUPL_RETURN_ERROR_EXPECTED)
//            {
//                m_wait.Wait();
//                continue;
//            }
//            else
//            {
//                DisplayMsg("Wrong output index, out of boudn", "Wrong arg", DXGI_ERROR_NOT_FOUND);
//                return QImage();
//            }
//        }
        //Reduce content, just test return once
    }
    return QImage();//temp
}
