#include "dx_VertexBuffer.h"
#include "dx_Utils.h"

DXVertexBuffer::~DXVertexBuffer()
{
	m_pBuffer->Release();
	m_pBuffer = nullptr;
}

DXVertexBuffer::DXVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* context, const std::vector<VS_INPUT>& vertices):
	m_pDevice(device), m_pDeviceContext(context)
{
	D3D11_BUFFER_DESC bufferDescription{};
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescription.ByteWidth = sizeof(VS_INPUT) * static_cast<uint32_t>(vertices.size());
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.CPUAccessFlags = 0;
	bufferDescription.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sr_data{};
	sr_data.pSysMem = vertices.data();

	HRESULT result = m_pDevice->CreateBuffer(&bufferDescription, &sr_data, &m_pBuffer);
	if (FAILED(result)) {
		PrintHResult(result);
	}
}

void DXVertexBuffer::Bind() const
{
	constexpr UINT stride = sizeof(VS_INPUT);
	constexpr UINT location = 0;
	constexpr UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0,1,&m_pBuffer,&stride,&offset);
}

void DXVertexBuffer::UnBind() const
{
	m_pDeviceContext->IASetVertexBuffers(0,1,nullptr, nullptr, nullptr);
}
