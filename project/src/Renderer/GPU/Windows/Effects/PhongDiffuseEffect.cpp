#include "PhongDiffuseEffect.h"
#include "../../../../Texture.h"
#include <iostream>

dae::PhongDiffuseEffect::PhongDiffuseEffect(ID3D11Device* pDevice,
    Texture* m_pDiffuseTexture,
    Texture* m_pNormalTexture,
    Texture* m_pGlossTexture,
    Texture* m_pSpecularTexture
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



void dae::PhongDiffuseEffect::SetDiffuseMap(Texture* pDiffuseTexture)
{
    if (m_pDiffuseMapVariable && pDiffuseTexture) {
        HRESULT result = m_pDiffuseMapVariable->SetResource(pDiffuseTexture->GetShaderResourceView());
        if (FAILED(result)) {
            std::cout << "failed to set DiffuseMap" << std::endl;
        }
    }
}

void dae::PhongDiffuseEffect::SetNormalMap(Texture* pNormalTexture)
{
    if (pNormalTexture) {
        HRESULT result = m_pNormalMapVariable->SetResource(pNormalTexture->GetShaderResourceView());
        if (FAILED(result)) {
            std::cout << "failed to set NormalMap" << std::endl;
        }
    }
}

void dae::PhongDiffuseEffect::SetGlossMap(Texture* pGlossTexture)
{
    if (pGlossTexture) {
        HRESULT result = m_pGlossMapVariable->SetResource(pGlossTexture->GetShaderResourceView());
        if (FAILED(result)) {
            std::cout << "failed to set GlossMap" << std::endl;
        }
    }
}

void dae::PhongDiffuseEffect::SetSpecularMap(Texture* pSpecularTexture)
{
    if (pSpecularTexture) {
        HRESULT result = m_pSpecularMapVariable->SetResource(pSpecularTexture->GetShaderResourceView());
        if (FAILED(result)) {
            std::cout << "failed to set SpecularMap" << std::endl;
        }
    }
}

dae::PhongDiffuseEffect::~PhongDiffuseEffect()
{

    m_pDiffuseTexture = nullptr;
    m_pNormalTexture = nullptr;
    m_pGlossTexture = nullptr;
    m_pSpecularTexture = nullptr;

    m_pDiffuseMapVariable->Release();
    m_pGlossMapVariable->Release();
    m_pNormalMapVariable->Release();
    m_pSpecularMapVariable->Release();

    m_pDiffuseMapVariable = nullptr;
    m_pGlossMapVariable = nullptr;
    m_pNormalMapVariable = nullptr;
    m_pSpecularMapVariable = nullptr;

}

void dae::PhongDiffuseEffect::SetupEffect()
{
    SetDiffuseMap(m_pDiffuseTexture);
    SetNormalMap(m_pNormalTexture);
    SetGlossMap(m_pGlossTexture);
    SetSpecularMap(m_pSpecularTexture);
}