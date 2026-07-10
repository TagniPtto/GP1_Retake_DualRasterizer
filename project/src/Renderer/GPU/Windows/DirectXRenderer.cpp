#include "DirectXRenderer.h"

#include <iostream>

void dae::DirectXRenderer::Render() const
{}

HRESULT dae::DirectXRenderer::InitializeDirectX()
{
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	uint8_t deviceFlags = 0;

#ifdef DEBUG
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result =D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_HARDWARE,0,deviceFlags,&featureLevel,1,D3D11_SDK_VERSION,&m_pDevice,nullptr,&m_pDeviceContext);
	if (FAILED(result)) {
        PrintHResult(result);
	}

}

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