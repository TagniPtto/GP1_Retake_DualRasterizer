#pragma once

#include <unordered_map>

#include "../../IRenderer.h"
#include "dx_Mesh.h"
#include "dx_Material.h"


#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <d3dx11effect.h>


class Mesh;
class Material;

class DXRenderer : public IRenderer
{
	private:
		
		SDL_Window* m_windowHandle;
		int m_Width{};
		int m_Height{};

		std::unordered_map<Mesh*,		std::unique_ptr<DXMesh>> m_meshResources;
		std::unordered_map<Material*,	std::unique_ptr<DXMaterial >> m_materialResources;

	
		ID3D11Device* m_pDevice;  					
		ID3D11DeviceContext* m_pDeviceContext;
		
		IDXGISwapChain* m_pSwapChain;

		ID3D11Texture2D* m_pDepthStencilBuffer;
		ID3D11DepthStencilView* m_pDepthStencilView;

		ID3D11Texture2D* m_pRenderTargetBuffer;
		ID3D11RenderTargetView* m_pRenderTargetView;

		float m_clearColor[4] = { .39f, .59f, .93f , 1.f };
		uint32_t m_techniqueIndex{};

	public:
		virtual ~DXRenderer() override;
		virtual void Render() const override;
		virtual void DrawMesh(const Mesh& mesh) const override;
		virtual void Update(const dae::Timer* pTimer) override;
		void Initialize(SDL_Window* handle);

	private:
		HRESULT CreateDeviceAndContext();
		HRESULT CreateSwapChain();
		HRESULT CreateDepthStencil();
		HRESULT CreateRenderTargetView();
		HRESULT BindBuffersToOutputMerger();
		HRESULT SetRasterizerViewport();
};