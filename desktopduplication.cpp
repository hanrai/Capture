#include <windows.h>
#include "desktopduplication.h"
#include <QtDebug>
#include <dxgi1_6.h>

DesktopDuplication::DesktopDuplication(QObject *parent) :
    QObject(parent),
    m_initialized(false)
{
    Init();
    //takeSnapshots();
}

void DesktopDuplication::Init()
{
    Reset();
    //
    //Create a DXGI Factory first.
    //
    ComPtr<IDXGIFactory1> pFactory;     //DXGI Factory
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
    if(SUCCEEDED(hr))
    {
        qDebug()<<"Factory interface object created.";

        //
        //Enumeration adapters installed in this machine.
        //
        ComPtr<IDXGIAdapter> pAdapter;
        for(int i=0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; i++)
        {
            DXGI_ADAPTER_DESC desc;
            hr = pAdapter->GetDesc(&desc);
            if (SUCCEEDED(hr))
            {
                qDebug()<<"Adapter:"<<QString::fromWCharArray(desc.Description);
                ComPtr<IDXGIOutput> pOutput;
                for(int i=0; pAdapter->EnumOutputs(i, &pOutput) != DXGI_ERROR_NOT_FOUND; i++)
                {
                    DXGI_OUTPUT_DESC desc;
                    pOutput->GetDesc(&desc);
                    qDebug()<<"Output:"<<QString::fromWCharArray(desc.DeviceName);

                    ComPtr<IDXGIOutput1> pOutput1;
                    hr = pOutput.As(&pOutput1);
                    if(SUCCEEDED(hr))
                    {
                        //
                        //Create D3D12 device
                        //
                        ComPtr<ID3D11Device> pDevice;
                        ComPtr<ID3D11DeviceContext> pContext;
                        D3D_FEATURE_LEVEL FeatureLevel;
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
                                    &pDevice,                   //ppDevice
                                    &FeatureLevel,              //pFeatureLevel
                                    &pContext);                 //context
                        if(SUCCEEDED(hr))
                        {
                            qDebug()<<"D3DDevice created:"<<(void*)FeatureLevel;

                            m_outputs.push_back(pOutput1);
                            m_devices.push_back(pDevice);
                            m_contexts.push_back(pContext);
                        }
                    }
                }
            }
        }
        qDebug()<<m_outputs.size()<<" outputs found.";
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

void DesktopDuplication::takeSnapshots()
{
    if(!m_initialized)
        return;

    m_snapshots.clear();

    for(int i=0; i<m_outputs.size(); i++)
    {
        qDebug()<<"Create snapshot for output: "<<i;
        ComPtr<IDXGIOutputDuplication> pDuplication;
        auto pOutput = m_outputs.at(i);
        auto pDevice = m_devices.at(i);
        HRESULT hr;
        hr = pOutput->DuplicateOutput(pDevice.Get(), &pDuplication);
        if(SUCCEEDED(hr))
        {
            qDebug()<<"Duplication interface get.";

            DXGI_OUTDUPL_DESC duplDesc;
            pDuplication->GetDesc(&duplDesc);
            qDebug()<<"DesktopImageInSystemMemory:"<<(bool)duplDesc.DesktopImageInSystemMemory;

            DXGI_OUTDUPL_FRAME_INFO info;
            ComPtr<IDXGIResource> resource;
            hr = pDuplication->AcquireNextFrame(
                        3000,                        //TimeoutInMilliseconds
                        &info,                      //pFrameInfo
                        &resource);                 //ppDesktopResource
            if(SUCCEEDED(hr))
            {
                qDebug()<<"Frame get.";
                D3D11_TEXTURE2D_DESC frameDescriptor;
                ComPtr<ID3D11Texture2D> frameTexture;
                hr = resource.As(&frameTexture);
                if(SUCCEEDED(hr))
                {
                    qDebug()<<"Texture of frame get.";
                    frameTexture->GetDesc(&frameDescriptor);
                    frameDescriptor.Usage = D3D11_USAGE_STAGING;
                    frameDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
                    frameDescriptor.BindFlags = 0;
                    frameDescriptor.MiscFlags = 0;
                    frameDescriptor.MipLevels = 1;
                    frameDescriptor.ArraySize = 1;
                    frameDescriptor.SampleDesc.Count = 1;

                    ComPtr<ID3D11Texture2D> stagingTexture;
                    hr = pDevice->CreateTexture2D(&frameDescriptor, NULL, &stagingTexture);
                    if(SUCCEEDED(hr))
                    {
                        qDebug()<<"Staging texture get.";
                        auto context = m_contexts.at(i);
                        context->CopyResource(stagingTexture.Get(), frameTexture.Get());
                        qDebug()<<"Resource copied.";

                        D3D11_MAPPED_SUBRESOURCE mapInfo;
                        hr = context->Map(
                                    stagingTexture.Get(),
                                    0,  // Subresource
                                    D3D11_MAP_READ,
                                    0,  // MapFlags
                                    &mapInfo);
                        if(SUCCEEDED(hr))
                        {
                            qDebug()<<"Texture mapped.";
                            auto result = QImage(
                                        frameDescriptor.Width,
                                        frameDescriptor.Height,
                                        QImage::Format_ARGB32);
                            for(int i=0; i<result.height(); i++)
                            {
                                memcpy(
                                    (void*)((char*)result.bits()+i*result.width()*4),
                                    (void*)((char*)mapInfo.pData+i*mapInfo.RowPitch),
                                    result.width()*4
                                      );
                            }
                            m_snapshots.push_back(result);
                        }
                        context->Unmap(stagingTexture.Get(), 0);
                    }
                }
            }
            pDuplication->ReleaseFrame();
        }
    }
}

void DesktopDuplication::Reset()
{
    m_initialized = false;
    m_contexts.clear();
    m_outputs.clear();
    m_devices.clear();
    m_duplications.clear();
    m_snapshots.clear();
}
