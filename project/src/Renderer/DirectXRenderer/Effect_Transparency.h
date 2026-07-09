#include "d3dx11effect.h"
#include <string>

#include "Matrix.h"

class Effect_Transparency {

public:
	Effect_Transparency(ID3D11Device* pDevice, const std::wstring& assetFile);
	~Effect_Transparency();
	ID3DX11Effect* GetEffectPtr();
	ID3DX11EffectTechnique* GetEffectTechniquePtr(int index);
	ID3D11InputLayout* GetInputLayoutPtr();


	static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	
	void SetTransparency(float transparency);

	void SetCameraPosition(const dae::Vector3& vector);
	void SetWorldViewProjectionMatrix(const dae::Matrix& matrix);
	void SetWorldMatrix(const dae::Matrix& matrix);
};
