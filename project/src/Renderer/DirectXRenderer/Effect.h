#pragma once
#include "d3dx11effect.h"
#include <string>

#include "Matrix.h"


class Texture;
class BaseEffect {

protected:
	ID3DX11Effect* m_pEffect{ nullptr };
	ID3D11InputLayout* m_pInputLayout{ nullptr };

	static constexpr int techniqueCount = 3;
	ID3DX11EffectTechnique* m_pEffectTechniques[techniqueCount];

	ID3DX11EffectVectorVariable* m_pCameraPositionVariable{ nullptr };

	ID3DX11EffectMatrixVariable* m_pWorldViewProjMatrixVariable{ nullptr };
	ID3DX11EffectMatrixVariable* m_pWorldMatrixVariable{ nullptr };

public:
	BaseEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	~BaseEffect();

	ID3DX11Effect* GetEffectPtr();
	ID3DX11EffectTechnique* GetEffectTechniquePtr(int index);
	ID3D11InputLayout* GetInputLayoutPtr();

	static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);

	virtual void SetupEffect() {};
	void SetCameraPosition(const dae::Vector3& vector);
	void SetWorldViewProjectionMatrix(const dae::Matrix& matrix);
	void SetWorldMatrix(const dae::Matrix& matrix);
};

class PhongDiffuseEffect : public BaseEffect {
private:

	Texture*  m_pDiffuseTexture;
	Texture*  m_pNormalTexture;
	Texture*  m_pGlossTexture;
	Texture*  m_pSpecularTexture;


	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{ nullptr };
	ID3DX11EffectShaderResourceVariable* m_pNormalMapVariable{ nullptr };
	ID3DX11EffectShaderResourceVariable* m_pGlossMapVariable{ nullptr };
	ID3DX11EffectShaderResourceVariable* m_pSpecularMapVariable{ nullptr };
public:
	PhongDiffuseEffect(
		ID3D11Device* pDevice, 
		Texture* m_pDiffuseTexture,
		Texture* m_pNormalTexture,
		Texture* m_pGlossTexture,
		Texture* m_pSpecularTexture);
	~PhongDiffuseEffect();
	virtual void SetupEffect() override {
		SetDiffuseMap(m_pDiffuseTexture);
		SetNormalMap(m_pNormalTexture);
		SetGlossMap(m_pGlossTexture);
		SetSpecularMap(m_pSpecularTexture);

	}

	void SetDiffuseMap(Texture* pDiffuseTexture);
	void SetNormalMap(Texture* pNormalTexture);
	void SetGlossMap(Texture* pGlossTexture);
	void SetSpecularMap(Texture* pSpecularTexture);

};

class TransparencyEffect : public BaseEffect {
private:
	Texture* m_pDiffuseTexture;

	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{ nullptr };
public:
	TransparencyEffect(ID3D11Device* pDevice,
		Texture* diffuseTexture);
	~TransparencyEffect();
	virtual void SetupEffect() override {
		SetDiffuseMap(m_pDiffuseTexture);

	}
	void SetDiffuseMap(Texture* pDiffuseTexture);
};

