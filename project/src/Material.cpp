#include "Material.h"


Texture* Material::GetDiffuseTexture()
{
	return m_pDiffuseTexture;
}

Texture* Material::GetNormalTexture()
{
	return m_pNormalTexture;
}

Texture* Material::GetGlossTexture()
{
	return m_pGlossTexture;
}

Texture* Material::GetSpecularTexture()
{
	return m_pSpecularTexture;
}
