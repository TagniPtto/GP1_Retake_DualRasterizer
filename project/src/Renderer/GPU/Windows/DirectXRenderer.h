#pragma once

#include "../../IRenderer.h"
#include <unordered_map>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <d3dx11effect.h>

namespace dae
{
	class DirectXRenderer : public IRenderer {
	private:
		
		SDL_Window* m_windowHandle;
		int m_Width{};
		int m_Height{};

		//std::unordered_map<uint64_t, > m_meshResources;
		//std::unordered_map<uint64_t, > m_textureResources;
		//std::unordered_map<uint64_t, > m_effectResources;


		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;
		IDXGISwapChain* m_pSwapChain;

		ID3D11Texture2D* m_pDepthStencilBuffer;
		ID3D11DepthStencilView* m_pDepthStencilView;

		ID3D11Texture2D* m_pRenderTargetBuffer;
		ID3D11RenderTargetView* m_pRenderTargetView;

	public:
		DirectXRenderer() = default;
		virtual ~DirectXRenderer() override = default;
		virtual void Render() const override;
		virtual void Update(const Timer* pTimer) override;
		void Initialize(SDL_Window* handle);

	private:

		HRESULT CreateDeviceAndContext();
		HRESULT CreateSwapChain();
		HRESULT CreateDepthStencil();
		HRESULT SetupRenderTargetView();
	};
}