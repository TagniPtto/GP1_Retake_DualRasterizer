#include "PhongDiffuseEffect.h"
#include "../../../../Texture.h"
#include <iostream>

dae::PhongDiffuseEffect::PhongDiffuseEffect(ID3D11Device* pDevice,
    DXTexture* m_pDiffuseTexture,
    DXTexture* m_pNormalTexture,
    DXTexture* m_pGlossTexture,
    DXTexture* m_pSpecularTexture
) : BaseEffect::BaseEffect(pDevice, std::wstring(L"resources/DiffusePhong.fx"))
{

    //Getting Texture variables on GPU side
    m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
    if (!m_pDiffuseMapVariable->IsValid()) {
        std::cout << "Failed to retrieve diffuseMap Variable \n";
    }
    m_pNormalMapVariable = m_pEffect->GetVariableByName("gNormalMap")->AsShaderResource();
    if (!m_pNormalMapVariable->IsValid()) {
        std::cout << "Failed to retrieve normalMap Variable \n";
    }
    m_pGlossMapVariable = m_pEffect->GetVariableByName("gGlossMap")->AsShaderResource();
    if (!m_pGlossMapVariable->IsValid()) {
        std::cout << "Failed to retrieve glossMap Variable \n";
    }
    m_pSpecularMapVariable = m_pEffect->GetVariableByName("gSpecularMap")->AsShaderResource();
    if (!m_pSpecularMapVariable->IsValid()) {
        std::cout << "Failed to retrieve specularMap Variable \n";
    }

    //m_pEffectTechniquePoint = m_pEffect->GetTechniqueByIndex(0);
    SetDiffuseMap(m_pDiffuseTexture);
    SetNormalMap(m_pNormalTexture);
    SetGlossMap(m_pGlossTexture);
    SetSpecularMap(m_pSpecularTexture);

}



void dae::PhongDiffuseEffect::SetDiffuseMap(DXTexture* pDiffuseTexture)
{
    if (m_pDiffuseMapVariable && pDiffuseTexture) {
        HRESULT result = m_pDiffuseMapVariable->SetResource(pDiffuseTexture->GetShaderResourceView());
        if (FAILED(result)) {
            std::cout << "failed to set DiffuseMap" << std::endl;
        }
    }
}

void dae::PhongDiffuseEffect::SetNormalMap(DXTexture* pNormalTexture)
{
    if (pNormalTexture) {
        HRESULT result = m_pNormalMapVariable->SetResource(pNormalTexture->GetShaderResourceView());
        if (FAILED(result)) {
            std::cout << "failed to set NormalMap" << std::endl;
        }
    }
}

void dae::PhongDiffuseEffect::SetGlossMap(DXTexture* pGlossTexture)
{
    if (pGlossTexture) {
        HRESULT result = m_pGlossMapVariable->SetResource(pGlossTexture->GetShaderResourceView());
        if (FAILED(result)) {
            std::cout << "failed to set GlossMap" << std::endl;
        }
    }
}

void dae::PhongDiffuseEffect::SetSpecularMap(DXTexture* pSpecularTexture)
{
    if (pSpecularTexture) {
        HRESULT result = m_pSpecularMapVariable->SetResource(pSpecularTexture->GetShaderResourceView());
        if (FAILED(result)) {
            std::cout << "failed to set SpecularMap" << std::endl;
        }
    }
}


void dae::PhongDiffuseEffect::SetupEffect()
{
    SetDiffuseMap(m_pDiffuseTexture);
    SetNormalMap(m_pNormalTexture);
    SetGlossMap(m_pGlossTexture);
    SetSpecularMap(m_pSpecularTexture);
}