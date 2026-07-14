#include "dx_IndexBuffer.h"
#include "dx_Utils.h"


DXIndexBuffer::~DXIndexBuffer()
{
	m_pBuffer->Release();
	m_pBuffer = nullptr;
}

DXIndexBuffer::DXIndexBuffer(ID3D11Device* device, ID3D11DeviceContext* context, const std::vector<uint32_t>& indices):
	m_pDevice(device),m_pDeviceContext(context)
{
	D3D11_BUFFER_DESC bufferDescription{};
	bufferDescription.ByteWidth = sizeof(uint32_t) * static_cast<uint32_t>(indices.size());
	bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.CPUAccessFlags = 0;
	bufferDescription.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sr_data{};
	sr_data.pSysMem = indices.data();

	HRESULT result = m_pDevice->CreateBuffer(&bufferDescription, &sr_data, &m_pBuffer);
	if (FAILED(result)) {
		PrintHResult(result);
	}
}

void DXIndexBuffer::Bind() const
{
	m_pDeviceContext->IASetIndexBuffer(m_pBuffer, DXGI_FORMAT_R32_UINT,0);
}

void DXIndexBuffer::UnBind() const
{
	m_pDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT,0);
}
