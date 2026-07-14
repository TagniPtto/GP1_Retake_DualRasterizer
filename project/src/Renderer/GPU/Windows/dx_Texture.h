#pragma once

#include "d3d11.h"


class DXTexture {
private:
	ID3D11Texture2D* m_pTexture;
	ID3D11ShaderResourceView* m_pShaderResouceView;
public:
	explicit DXTexture(ID3D11Device* pDevice);
	virtual ~DXTexture();
	ID3D11ShaderResourceView* GetShaderResourceView();

};