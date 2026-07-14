#pragma once

#include "BaseEffect.h"

#include "../dx_Texture.h"

namespace dae {
	class PhongDiffuseEffect : public BaseEffect {
	private:

		DXTexture* m_pDiffuseTexture;
		DXTexture* m_pNormalTexture;
		DXTexture* m_pGlossTexture;
		DXTexture* m_pSpecularTexture;


		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{ nullptr };
		ID3DX11EffectShaderResourceVariable* m_pNormalMapVariable{ nullptr };
		ID3DX11EffectShaderResourceVariable* m_pGlossMapVariable{ nullptr };
		ID3DX11EffectShaderResourceVariable* m_pSpecularMapVariable{ nullptr };
	public:
		virtual ~PhongDiffuseEffect() = default;
		PhongDiffuseEffect(
			ID3D11Device* pDevice,
			DXTexture* m_pDiffuseTexture,
			DXTexture* m_pNormalTexture,
			DXTexture* m_pGlossTexture,
			DXTexture* m_pSpecularTexture);
		virtual void SetupEffect() override;
		void SetDiffuseMap(DXTexture* pDiffuseTexture);
		void SetNormalMap(DXTexture* pNormalTexture);
		void SetGlossMap(DXTexture* pGlossTexture);
		void SetSpecularMap(DXTexture* pSpecularTexture);

	};
}