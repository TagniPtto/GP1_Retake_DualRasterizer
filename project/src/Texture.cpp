#include "Texture.h"

Texture::Texture(const std::string& textureSource)
{
	m_pSurface = IMG_Load(textureSource.c_str());
	if (m_pSurface == nullptr) {
		std::cout << "failed to load texture : " << textureSource << std::endl;
		return;
	}

	m_pSurfacePixels =  (uint32_t*)m_pSurface->pixels;
}

Texture::~Texture()
{
	SDL_FreeSurface(m_pSurface);
}
dae::ColorRGB Texture::Sample(const dae::Vector2& uv) const
{

	if (uv.x > 1 || uv.y > 1 || uv.x < 0 || uv.y < 0) {
		return{};
	}

	const int x{ int(uv.x * m_pSurface->w) };
	const int y{ int(uv.y * m_pSurface->h) };
	uint8_t* c{ (uint8_t*)&m_pSurfacePixels[x + y * m_pSurface->w] };

	return dae::ColorRGB{ c[0] / 255.f,c[1] / 255.f,c[2] / 255.f };
}