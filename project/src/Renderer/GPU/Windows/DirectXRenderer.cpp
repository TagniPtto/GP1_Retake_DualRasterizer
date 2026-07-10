#include "DirectXRenderer.h"

#include <iostream>

void PrintHResult(HRESULT hr)
{
    LPSTR message = nullptr;

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&message,
        0,
        nullptr);

    if (message)
    {
        std::cout << message << std::endl;
        LocalFree(message);
    }
}

HRESULT dae::DirectXRenderer::CreateDeviceAndContext()
{
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    uint8_t deviceFlags = 0;

#ifdef DEBUG
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, deviceFlags, &featureLevel, 1, D3D11_SDK_VERSION, &m_pDevice, nullptr, &m_pDeviceContext);
    if (FAILED(result)) {
        PrintHResult(result);
    }
    return result;
}

HRESULT dae::DirectXRenderer::CreateSwapChain()
{
    IDXGIFactory1* factory{};
    HRESULT result = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&factory));
    if (FAILED(result)) {
        PrintHResult(result);
        return result;
    }
    DXGI_SWAP_CHAIN_DESC swapChainDescriptor{};
    swapChainDescriptor.BufferDesc.Width = m_Width;
    swapChainDescriptor.BufferDesc.Height = m_Height;
    swapChainDescriptor.BufferDesc.RefreshRate.Numerator = 1;
    swapChainDescriptor.BufferDesc.RefreshRate.Denominator = 60;
    swapChainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDescriptor.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDescriptor.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDescriptor.SampleDesc.Count = 1;
    swapChainDescriptor.SampleDesc.Quality = 0;
    swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescriptor.BufferCount = 1;
    swapChainDescriptor.Windowed = true;
    swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDescriptor.Flags = 0;


    SDL_SysWMinfo sysWMinfo{};
    SDL_GetVersion(&sysWMinfo.version);
    SDL_GetWindowWMInfo(m_windowHandle, &sysWMinfo);

    swapChainDescriptor.OutputWindow = sysWMinfo.info.win.window;


    result = factory->CreateSwapChain(m_pDevice , &swapChainDescriptor, &m_pSwapChain);
    if (FAILED(result)) {
        PrintHResult(result);
    }
    return result;
}

HRESULT dae::DirectXRenderer::CreateDepthStencil()
{
    D3D11_TEXTURE2D_DESC depthStencilDescriptor{};
    depthStencilDescriptor.Width = m_Width;
    depthStencilDescriptor.Height = m_Height;
    depthStencilDescriptor.MipLevels = 1;
    depthStencilDescriptor.ArraySize = 1;
    depthStencilDescriptor.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDescriptor.SampleDesc.Count = 1;
    depthStencilDescriptor.SampleDesc.Quality = 0;
    depthStencilDescriptor.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDescriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDescriptor.CPUAccessFlags = 0;
    depthStencilDescriptor.MiscFlags = 0;


    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescriptor{};
    depthStencilViewDescriptor.Format = depthStencilDescriptor.Format;
    depthStencilViewDescriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDescriptor.Texture2D.MipSlice = 0;

    

    HRESULT result = m_pDevice->CreateTexture2D(&depthStencilDescriptor, nullptr, &m_pDepthStencilBuffer);
    if (FAILED(result)) {
        PrintHResult(result);
        return result;
    }


    result = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDescriptor, &m_pDepthStencilView);
    if (FAILED(result)) {
        PrintHResult(result);
    }
    return result;
}

HRESULT dae::DirectXRenderer::SetupRenderTargetView()
{

    HRESULT result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pRenderTargetBuffer));
    if (FAILED(result)) {
        PrintHResult(result);
        return result;
    }

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDescriptor{};
    result = m_pDevice->CreateRenderTargetView(m_pRenderTargetBuffer,&renderTargetViewDescriptor,&m_pRenderTargetView);
    if (FAILED(result)) {
        PrintHResult(result);
    }
    return result;
}

void dae::DirectXRenderer::Initialize(SDL_Window* handle)
{
    m_windowHandle = handle;
    SDL_GetWindowSize(m_windowHandle , &m_Width, &m_Height);

    CreateDeviceAndContext();
    CreateSwapChain();
    CreateDepthStencil();
    SetupRenderTargetView();
}
void dae::DirectXRenderer::Render() const
{
}

void dae::DirectXRenderer::Update(const Timer* pTimer)
{}
