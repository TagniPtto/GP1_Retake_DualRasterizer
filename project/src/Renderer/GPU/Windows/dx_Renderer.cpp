#include "dx_Renderer.h"
#include "dx_Utils.h"

#include <iostream>


HRESULT DXRenderer::CreateDeviceAndContext()
{
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    uint8_t deviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, deviceFlags, &featureLevel, 1, D3D11_SDK_VERSION, &m_pDevice, nullptr, &m_pDeviceContext);
    if (FAILED(result)) {
        PrintHResult(result);
    }

    return result;
}

HRESULT DXRenderer::CreateSwapChain()
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

HRESULT DXRenderer::CreateDepthStencil()
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

HRESULT DXRenderer::CreateRenderTargetView()
{

    HRESULT result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pRenderTargetBuffer));
    if (FAILED(result)) {
        PrintHResult(result);
        return result;
    }

    result = m_pDevice->CreateRenderTargetView(m_pRenderTargetBuffer,nullptr,&m_pRenderTargetView);
    if (FAILED(result)) {
        PrintHResult(result);
    }
    return result;
}

HRESULT DXRenderer::BindBuffersToOutputMerger()
{
    m_pDeviceContext->OMSetRenderTargets(1,&m_pRenderTargetView,m_pDepthStencilView);
    return S_OK;
}

HRESULT DXRenderer::SetRasterizerViewport()
{
    D3D11_VIEWPORT viewport{};
    viewport.Width = m_Width;
    viewport.Height = m_Height;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 1;
    viewport.MaxDepth = 1;

    m_pDeviceContext->RSSetViewports(1,&viewport);

    return S_OK;
}

void DXRenderer::Initialize(SDL_Window* handle)
{
    m_windowHandle = handle;
    SDL_GetWindowSize(m_windowHandle, &m_Width, &m_Height);

    std::cout << "[Renderer] Creating D3D11 Device ...\n";
    if (FAILED(CreateDeviceAndContext()))
    {
        std::cout << "[Renderer] Failed to Create D3D11 Device ...\n";
        return;
    }
    std::cout << "[Renderer] Creating D3D11 SwapChain ...\n";
    if (FAILED(CreateSwapChain()))
    {
        std::cout << "[Renderer] Failed to Create D3D11 SwapChain ...\n";
        return;
    }
    std::cout << "[Renderer] Creating D3D11 DepthStencil ...\n";
    if (FAILED(CreateDepthStencil()))
    {
        std::cout << "[Renderer] Creating D3D11 DepthStencil ...\n";
        return;
    }
    std::cout << "[Renderer] Creating D3D11 RenderTarget View...\n";
    if (FAILED(CreateRenderTargetView()))
    {
        std::cout << "[Renderer]  Failed to Create D3D11 RenderTarget View...\n";
        return;
    }

    std::cout << "[Renderer] Binding depthstencil and rendertarget views to OutputMerger...\n";
    BindBuffersToOutputMerger();
    std::cout << "[Renderer] Setting rasterizer viewport...\n";
    SetRasterizerViewport();

    std::cout << "[Renderer] D3D11 Initialization success...\n";
}
DXRenderer::~DXRenderer()
{
    if (m_pRenderTargetView)
    {
        m_pRenderTargetView->Release();
    }
    if (m_pRenderTargetBuffer)
    {
        m_pRenderTargetBuffer->Release();
    }
    if (m_pDepthStencilView)
    {
        m_pDepthStencilView->Release();
    }
    if (m_pDepthStencilBuffer)
    {
        m_pDepthStencilBuffer->Release();
    }
    if (m_pSwapChain)
    {
        m_pSwapChain->Release();
    }
    if (m_pDeviceContext) {
        m_pDeviceContext->ClearState();
        m_pDeviceContext->Flush();
        m_pDeviceContext->Release();
    }
    if (m_pDevice) {
        m_pDevice->Release();
    }


}
void DXRenderer::Render() const
{
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, m_clearColor);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView,D3D11_CLEAR_STENCIL| D3D11_CLEAR_DEPTH,1.0f,0);

    m_pSwapChain->Present(0,0);
}

void DXRenderer::DrawMesh(const Mesh& mesh) const
{
    const auto& dxMaterial = m_materialResources.at(mesh.GetMaterial());
    const auto& dxMesh = m_meshResources.at(&mesh);
    const auto effect = dxMaterial->GetEffectPtr();
    const auto inputLayout = effect->GetInputLayoutPtr();
    const auto topology = mesh.m_triangleTopology == PrimitiveTopology::TriangleStrip ? D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP : D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    
    m_pDeviceContext->IASetPrimitiveTopology(topology);
    m_pDeviceContext->IASetInputLayout(inputLayout);


    D3DX11_TECHNIQUE_DESC techniqueDescription{};

    HRESULT result = effect->GetEffectTechniquePtr(m_techniqueIndex)->GetDesc(&techniqueDescription);
    if (FAILED(result)) {
        PrintHResult(result);
    }
    for (UINT i{}; i < techniqueDescription.Passes; i++) {

        result = effect->GetEffectTechniquePtr(m_techniqueIndex)->GetPassByIndex(i)->Apply(0, m_pDeviceContext);
        if (FAILED(result)) {
            PrintHResult(result);
        }
        m_pDeviceContext->DrawIndexed(mesh.m_indices.size(), 0, 0);
    }
}

void DXRenderer::Update(const dae::Timer* pTimer)
{}
