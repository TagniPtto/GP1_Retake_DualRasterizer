#pragma once

#include "../IRenderer.h"
#include <unordered_map>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <d3dx11effect.h>

namespace dae
{
	class DirectXRenderer : public IRenderer {
	private:
		std::unordered_map<uint64_t, > m_meshResources;
		std::unordered_map<uint64_t, > m_textureResources;
		std::unordered_map<uint64_t, > m_effectResources;

		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;

	public:
		virtual ~DirectXRenderer() override;
		virtual void Render() const override;

	private:
		HRESULT InitializeDirectX();
	};
}