#pragma once

#include "d3d11.h"

#include <vector>

class DXIndexBuffer 
{
private:
	ID3D11Buffer* m_pBuffer{ nullptr };
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
public:
	virtual ~DXIndexBuffer();
	explicit DXIndexBuffer(
		ID3D11Device*,
		ID3D11DeviceContext*,
		const std::vector<uint32_t>&);
	virtual void Bind() const;
	virtual void UnBind() const;

};