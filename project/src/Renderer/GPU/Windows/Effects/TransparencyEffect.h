#include "BaseEffect.h"

namespace dae
{
	class TransparencyEffect : public BaseEffect {
	private:
		Texture* m_pDiffuseTexture;

		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{ nullptr };
	public:
		TransparencyEffect(ID3D11Device* pDevice,Texture* diffuseTexture);
		~TransparencyEffect();
		virtual void SetupEffect() override;
		void SetDiffuseMap(Texture* pDiffuseTexture);
	};


}
