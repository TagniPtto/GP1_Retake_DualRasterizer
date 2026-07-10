#include "TransparencyEffect.h"
#include "../../../../Texture.h"
#include <iostream>

dae::TransparencyEffect::TransparencyEffect(ID3D11Device* pDevice, Texture* diffuseTexture) :
    BaseEffect(pDevice, std::wstring(L"resources/Transparency.fx")), m_pDiffuseTexture(diffuseTexture)
{


    m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gTransparentDiffuseMap")->AsShaderResource();
    if (!m_pDiffuseMapVariable->IsValid()) {
        std::cout << "Failed to retrieve diffuseMap Variable \n";
    }
    //m_pEffectTechniquePoint = m_pEffect->GetTechniqueByIndex(0);
    SetDiffuseMap(m_pDiffuseTexture);
}

dae::TransparencyEffect::~TransparencyEffect()
{
    m_pDiffuseTexture = nullptr;

    m_pDiffuseMapVariable->Release();
    m_pDiffuseMapVariable = nullptr;
}

void dae::TransparencyEffect::SetupEffect()
{}

void dae::TransparencyEffect::SetDiffuseMap(Texture* pDiffuseTexture)
{
    if (m_pDiffuseMapVariable && pDiffuseTexture) {
        HRESULT result = m_pDiffuseMapVariable->SetResource(pDiffuseTexture->GetShaderResourceView());
        if (FAILED(result)) {
            std::cout << "failed to set DiffuseMap" << std::endl;
        }
    }
}
