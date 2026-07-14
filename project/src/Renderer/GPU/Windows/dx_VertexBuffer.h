#pragma once

#include "../../../DataTypes.h"
#include "d3d11.h"

#include <vector>

class DXVertexBuffer 
{
private:
	ID3D11Buffer* m_pBuffer{ nullptr };
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

public:
	virtual ~DXVertexBuffer();
	explicit DXVertexBuffer(
		ID3D11Device* device,
		ID3D11DeviceContext* context, 
		const std::vector<VS_INPUT>&);
	virtual void Bind() const;
	virtual void UnBind() const;

};