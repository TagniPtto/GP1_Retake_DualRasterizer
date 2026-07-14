#include "dx_Material.h"




DXMaterial::~DXMaterial()
{
	delete m_pEffect;
	m_pEffect = nullptr;
}

BaseEffect* DXMaterial::GetEffectPtr() const
{
	return m_pEffect;
}