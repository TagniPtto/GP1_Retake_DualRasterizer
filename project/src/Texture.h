#pragma once
#include <iostream>
#include "SDL_image.h"

#include "d3d11.h"

#include "Vector2.h"
#include "ColorRGB.h"

class Texture {
private:
	ID3D11Texture2D* m_pTexture;
	ID3D11ShaderResourceView* m_pShaderResouceView;

	//software
	SDL_Surface* m_pSurface{nullptr};
	uint32_t* m_pSurfacePixels{ nullptr };
public:
	ID3D11ShaderResourceView* GetShaderResourceView() {
		return m_pShaderResouceView;
	}

	dae::ColorRGB Sample(const dae::Vector2& uv) const;
	Texture(ID3D11Device* pDevice, const std::string& textureSource);
	~Texture();


};