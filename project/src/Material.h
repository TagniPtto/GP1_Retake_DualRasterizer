#pragma once

#include "Texture.h"



class Material {
private:
	Texture* m_pDiffuseTexture{ nullptr };
	Texture* m_pNormalTexture{ nullptr };
	Texture* m_pGlossTexture{ nullptr };
	Texture* m_pSpecularTexture{ nullptr };

public :
	virtual ~Material() = default;

	Texture* GetDiffuseTexture();
	Texture* GetNormalTexture();
	Texture* GetGlossTexture();
	Texture* GetSpecularTexture();

};