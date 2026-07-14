#include "BaseEffect.h"
#include "../dx_Texture.h"
namespace dae
{
	class TransparencyEffect : public BaseEffect {
	private:
		DXTexture* m_pDiffuseTexture;

		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{ nullptr };
	public:
		TransparencyEffect(ID3D11Device* pDevice, DXTexture* diffuseTexture);
		~TransparencyEffect();
		virtual void SetupEffect() override;
		void SetDiffuseMap(DXTexture* pDiffuseTexture);
	};


}
