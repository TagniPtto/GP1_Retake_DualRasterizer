#pragma once
#include <iostream>
#include "SDL_image.h"


#include "Math/Vector2.h"
#include "ColorRGB.h"

class Texture {
private:
	SDL_Surface* m_pSurface{nullptr};
	uint32_t* m_pSurfacePixels{ nullptr };
public:
	dae::ColorRGB Sample(const dae::Vector2& uv) const;
	Texture( const std::string& textureSource);
	~Texture();
};