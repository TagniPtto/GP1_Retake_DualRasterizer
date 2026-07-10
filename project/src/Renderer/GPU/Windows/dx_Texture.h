#include "../gpu_Texture.h"


#include "d3d11.h"


class DXTexture : public GPUTexture {
private:
	ID3D11Texture2D* m_pTexture;
	ID3D11ShaderResourceView* m_pShaderResouceView;
public:
	virtual ~DXTexture() = default;


};