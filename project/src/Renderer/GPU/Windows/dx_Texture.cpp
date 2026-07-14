#include "dx_Texture.h"

DXTexture::DXTexture(ID3D11Device* pDevice)
{
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
	D3D11_TEXTURE2D_DESC desc{};
	desc.Format = format;
	desc.Width = m_pSurface->w;
	desc.Height = m_pSurface->h;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.CPUAccessFlags = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	//https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_subresource_data
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = m_pSurface->pixels;
	//sysmemPitch distance in bytes from on row of pixels
	initData.SysMemPitch = static_cast<UINT>(m_pSurface->pitch);
	//SysMemSlicePitch distance in bytes from one slice of pixels in a 3d texture not really used in 2d or 1d textures
	initData.SysMemSlicePitch = static_cast<UINT>(m_pSurface->pitch * m_pSurface->h);

	HRESULT result = pDevice->CreateTexture2D(&desc, &initData, &m_pTexture);


	if (FAILED(result)) {
		std::cout << "Failed to Create Texture 2D" << std::endl;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc{};
	resourceViewDesc.Format = format;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MipLevels = 1;
	result = pDevice->CreateShaderResourceView(m_pTexture, &resourceViewDesc, &m_pShaderResouceView);
	if (FAILED(result)) {
		std::cout << "Failed to Create Shader Resouce view for texture" << std::endl;
	}
}

DXTexture::~DXTexture()
{
	m_pTexture->Release();
	m_pShaderResouceView->Release();
	m_pTexture = nullptr;
	m_pShaderResouceView = nullptr;
}

ID3D11ShaderResourceView* DXTexture::GetShaderResourceView()
{
	return m_pShaderResouceView;
}
