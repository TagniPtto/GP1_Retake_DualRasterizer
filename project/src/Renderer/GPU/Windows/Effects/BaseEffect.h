#pragma once
#include "d3dx11effect.h"
#include <string>

#include "../../../../Math/Math.h"


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
	virtual ~BaseEffect();

	ID3DX11Effect* GetEffectPtr();
	ID3DX11EffectTechnique* GetEffectTechniquePtr(int index);
	ID3D11InputLayout* GetInputLayoutPtr();

	static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);

	virtual void SetupEffect() {};
	void SetCameraPosition(const dae::Vector3& vector);
	void SetWorldViewProjectionMatrix(const dae::Matrix& matrix);
	void SetWorldMatrix(const dae::Matrix& matrix);
};



