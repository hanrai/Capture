#include <windows.h>
#include "desktopduplication.h"
#include <QtDebug>
#include <dxgi1_6.h>

DesktopDuplication::DesktopDuplication(QObject *parent) :
    QObject(parent)
{
    Init();
}

void DesktopDuplication::Init()
{
    Reset();
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

void DesktopDuplication::Reset()
{
    m_initialized = false;
    m_outputs.clear();
    m_outputID = -1;
    m_device.Reset();
    m_context.Reset();
    m_duplication.Reset();
    m_stagingTexture.Reset();
}

bool DesktopDuplication::setOutputID(int id)
{
    if(!m_initialized)
        Init();

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
                    D3D11_CREATE_DEVICE_DEBUG,  //Flags
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
                qDebug()<<"Texture mapped.";
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
    }
    return m_snapshot;
}
