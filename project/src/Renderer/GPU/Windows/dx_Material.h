#pragma once

#include "Effects/BaseEffect.h"

class DXMaterial
{
private:
	BaseEffect* m_pEffect{ nullptr };

public:
	virtual ~DXMaterial();
	BaseEffect* GetEffectPtr();
};