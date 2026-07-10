#pragma once

#include "BaseEffect.h"

namespace dae {
	class PhongDiffuseEffect : public BaseEffect {
	private:

		Texture* m_pDiffuseTexture;
		Texture* m_pNormalTexture;
		Texture* m_pGlossTexture;
		Texture* m_pSpecularTexture;


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
		virtual ~PhongDiffuseEffect();
		virtual void SetupEffect() override;
		void SetDiffuseMap(Texture* pDiffuseTexture);
		void SetNormalMap(Texture* pNormalTexture);
		void SetGlossMap(Texture* pGlossTexture);
		void SetSpecularMap(Texture* pSpecularTexture);

	};
}