#include "Effect.h"
#include "Texture.h"

#include <sstream>
#include <iostream>


BaseEffect::BaseEffect(ID3D11Device* pDevice, const std::wstring& assetFile) :
    m_pEffect(BaseEffect::LoadEffect(pDevice, assetFile)), m_pInputLayout(nullptr)

{

    const std::string techniqueName0{ "TechniquePointSampling" };
    const std::string techniqueName1{ "TechniqueLinearSampling" };
    const std::string techniqueName2{ "TechniqueAnisotropicSampling" };

    m_pEffectTechniques[0] = m_pEffect->GetTechniqueByName(techniqueName0.c_str());
    m_pEffectTechniques[1] = m_pEffect->GetTechniqueByName(techniqueName1.c_str());
    m_pEffectTechniques[2] = m_pEffect->GetTechniqueByName(techniqueName2.c_str());

    if (!m_pEffectTechniques[0]->IsValid()) {
        std::cout << "Failed to retrieve technique : " << techniqueName0 << std::endl;
    }
    if (!m_pEffectTechniques[1]->IsValid()) {
        std::cout << "Failed to retrieve technique : " << techniqueName1 << std::endl;
    }
    if (!m_pEffectTechniques[2]->IsValid()) {
        std::cout << "Failed to retrieve technique : " << techniqueName2 << std::endl;
    }

    // Creating Vertex Layout;
    static constexpr uint32_t numElements{ 5 };
    D3D11_INPUT_ELEMENT_DESC vertexDescription[numElements]{};

    // Defining the vertex attributes location, format slotclass and name
    vertexDescription[0].SemanticName = "POSITION";
    vertexDescription[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    vertexDescription[0].AlignedByteOffset = 0;
    vertexDescription[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    vertexDescription[1].SemanticName = "COLOR";
    vertexDescription[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    vertexDescription[1].AlignedByteOffset = 3 * 4; // POSITION ATTRIBUTE IS 3 * 4 bytes (32 bit)
    vertexDescription[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    vertexDescription[2].SemanticName = "TEXCOORD";
    vertexDescription[2].Format = DXGI_FORMAT_R32G32_FLOAT;
    vertexDescription[2].AlignedByteOffset = 6 * 4;
    vertexDescription[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    vertexDescription[3].SemanticName = "NORMAL";
    vertexDescription[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    vertexDescription[3].AlignedByteOffset = 8 * 4;
    vertexDescription[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    vertexDescription[4].SemanticName = "TANGENT";
    vertexDescription[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    vertexDescription[4].AlignedByteOffset = 11 * 4;
    vertexDescription[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;



    D3DX11_PASS_DESC passDescription;
    ID3DX11EffectPass* pass = GetEffectTechniquePtr(0)->GetPassByIndex(0);
    HRESULT result = pass->GetDesc(&passDescription);

    if (FAILED(result)) {
        std::cout << " Failed to GetTechniqueDescription\n";
    }

    result = pDevice->CreateInputLayout(vertexDescription, numElements, passDescription.pIAInputSignature, passDescription.IAInputSignatureSize, &m_pInputLayout);
    if (FAILED(result)) {
        std::cout << "failed to create InputLayout\n";
        return;
    }

    //Getting Matrix variables on GPU side

    m_pCameraPositionVariable = m_pEffect->GetVariableByName("gCameraPosition")->AsVector();
    if (!m_pCameraPositionVariable->IsValid()) {
        std::cout << "Failed to retrieve CameraPosition Variable \n";
    }
    m_pWorldViewProjMatrixVariable = m_pEffect->GetVariableByName("gWorldViewProjection")->AsMatrix();
    if (!m_pWorldViewProjMatrixVariable->IsValid()) {
        std::cout << "Failed to retrieve WorldViewMatrix Variable \n";
    }
    m_pWorldMatrixVariable = m_pEffect->GetVariableByName("gWorldMatrix")->AsMatrix();
    if (!m_pWorldMatrixVariable->IsValid()) {
        std::cout << "Failed to retrieve WorldMatrix Variable \n";
    }
}

ID3DX11Effect* BaseEffect::LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
{
    HRESULT result;

    ID3DX11Effect* effect;
    ID3D10Blob* errorBlob{nullptr};

    DWORD shaderFlags = 0;

#define DEBUG
#if defined(DEBUG) || defined(_DEBUG)  
    shaderFlags |= D3D10_SHADER_DEBUG;
    shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
    result = D3DX11CompileEffectFromFile(assetFile.c_str(),nullptr, nullptr, shaderFlags,0,pDevice,&effect,&errorBlob );

    if (FAILED(result)) {
        if (errorBlob != nullptr) {
            const char* pError = static_cast<char*>(errorBlob->GetBufferPointer());
            std::wstringstream ss;
            for (unsigned int i{}; i < errorBlob->GetBufferSize(); i++) {
                ss << pError[i];
            }
            OutputDebugStringW(ss.str().c_str());
            errorBlob->Release();
            errorBlob = nullptr;


            std::wcout << ss.str() << std::endl;
        }
        else {
            std::wstringstream ss;
            ss << L"failed to load file : " << assetFile << std::endl;
            std::wcout << ss.str() << std::endl;
        }
    }
    
    return effect;
}

void BaseEffect::SetCameraPosition(const dae::Vector3& vector)
{
    if (m_pCameraPositionVariable) {
        HRESULT result = m_pCameraPositionVariable->SetFloatVector((const float*)&vector);
        if (FAILED(result)) {
            std::cout << "failed to set CameraPosition" << std::endl;
        }
    }

}

void BaseEffect::SetWorldViewProjectionMatrix(const dae::Matrix& matrix)
{
    if (m_pWorldViewProjMatrixVariable) {
        HRESULT result = m_pWorldViewProjMatrixVariable->SetMatrix((const float*)(&matrix));
        if (FAILED(result)) {
            std::cout << "failed to set WorldViewProjectionMatrix" << std::endl;
        }
    }

}

void BaseEffect::SetWorldMatrix(const dae::Matrix& matrix)
{
    if (m_pWorldMatrixVariable) {
        HRESULT result = m_pWorldMatrixVariable->SetMatrix((const float*)(&matrix));
        if (FAILED(result)) {
            std::cout << "failed to set WorldMatrix" << std::endl;
        }
    }

}

BaseEffect::~BaseEffect()
{
    m_pEffect->Release();


    for (ID3DX11EffectTechnique*& technique : m_pEffectTechniques) {
        technique->Release();
        technique = nullptr;
    }


    m_pInputLayout->Release();



    m_pWorldViewProjMatrixVariable->Release();
    m_pWorldMatrixVariable->Release();
    m_pCameraPositionVariable->Release();

    m_pEffect = nullptr;
    m_pInputLayout = nullptr;

    m_pWorldViewProjMatrixVariable = nullptr;
    m_pWorldMatrixVariable = nullptr;
    m_pCameraPositionVariable = nullptr;
}

ID3DX11Effect* BaseEffect::GetEffectPtr()
{
    return m_pEffect;
}

ID3DX11EffectTechnique* BaseEffect::GetEffectTechniquePtr(int index)
{
    return m_pEffectTechniques[index % techniqueCount];
}

ID3D11InputLayout* BaseEffect::GetInputLayoutPtr()
{
    return m_pInputLayout;
}


PhongDiffuseEffect::PhongDiffuseEffect(ID3D11Device* pDevice,
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


void PhongDiffuseEffect::SetDiffuseMap(Texture* pDiffuseTexture)
{
    if (m_pDiffuseMapVariable && pDiffuseTexture) {
        HRESULT result = m_pDiffuseMapVariable->SetResource(pDiffuseTexture->GetShaderResourceView());
        if (FAILED(result)) {
            std::cout << "failed to set DiffuseMap" << std::endl;
        }
    }
}

void PhongDiffuseEffect::SetNormalMap(Texture* pNormalTexture)
{
    if (pNormalTexture) {
        HRESULT result = m_pNormalMapVariable->SetResource(pNormalTexture->GetShaderResourceView());
        if (FAILED(result)) {
            std::cout << "failed to set NormalMap" << std::endl;
        }
    }
}

void PhongDiffuseEffect::SetGlossMap(Texture* pGlossTexture)
{
    if (pGlossTexture) {
        HRESULT result = m_pGlossMapVariable->SetResource(pGlossTexture->GetShaderResourceView());
        if (FAILED(result)) {
            std::cout << "failed to set GlossMap" << std::endl;
        }
    }
}

void PhongDiffuseEffect::SetSpecularMap(Texture* pSpecularTexture)
{
    if (pSpecularTexture) {
        HRESULT result = m_pSpecularMapVariable->SetResource(pSpecularTexture->GetShaderResourceView());
        if (FAILED(result)) {
            std::cout << "failed to set SpecularMap" << std::endl;
        }
    }
}

PhongDiffuseEffect::~PhongDiffuseEffect()
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

TransparencyEffect::TransparencyEffect(ID3D11Device* pDevice, Texture* diffuseTexture):
    BaseEffect(pDevice , std::wstring(L"resources/Transparency.fx")), m_pDiffuseTexture(diffuseTexture)
{


    m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gTransparentDiffuseMap")->AsShaderResource();
    if (!m_pDiffuseMapVariable->IsValid()) {
        std::cout << "Failed to retrieve diffuseMap Variable \n";
    }
    //m_pEffectTechniquePoint = m_pEffect->GetTechniqueByIndex(0);
    SetDiffuseMap(m_pDiffuseTexture);
}

TransparencyEffect::~TransparencyEffect()
{
    m_pDiffuseTexture = nullptr;

    m_pDiffuseMapVariable->Release();
    m_pDiffuseMapVariable = nullptr;
}

void TransparencyEffect::SetDiffuseMap(Texture* pDiffuseTexture)
{
    if (m_pDiffuseMapVariable && pDiffuseTexture) {
        HRESULT result = m_pDiffuseMapVariable->SetResource(pDiffuseTexture->GetShaderResourceView());
        if (FAILED(result)) {
            std::cout << "failed to set DiffuseMap" << std::endl;
        }
    }
}
