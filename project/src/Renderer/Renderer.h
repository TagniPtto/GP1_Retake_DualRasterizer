#pragma once


//---Directx_Headers---------
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <d3dx11effect.h>

#include <memory>
#include "../Mesh.h"
#include "../Camera.h"
#include "GPU/Windows/Effects/BaseEffect.h"
#include "GPU/Windows/Effects/PhongDiffuseEffect.h"
#include "GPU/Windows/Effects/TransparencyEffect.h"


struct SDL_Window;
struct SDL_Surface;
class Texture;

namespace dae
{
	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Update(const Timer* pTimer);
		void Render() const;
		void SoftwareRender() const;
		void HardwareRender() const;

	private:
		SDL_Window* m_pWindow{};

		int m_Width{};
		int m_Height{};

		bool m_IsInitialized{ false };

		//HardWare Raserizer
	private:

		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;
		
		IDXGISwapChain* m_pSwapChain;
		
		ID3D11Texture2D* m_pDepthStencilBuffer;
		ID3D11DepthStencilView* m_pDepthStencilView;
		
		ID3D11Texture2D* m_pRenderTargetBuffer;
		ID3D11RenderTargetView* m_pRenderTargetView;


		Texture* m_pFireDiffuseTexture{ nullptr };

		Texture* m_pVehicleDiffuseTexture{ nullptr };
		Texture* m_pVehicleNormalTexture{ nullptr };
		Texture* m_pVehicleGlossTexture{ nullptr };
		Texture* m_pVehicleSpecularTexture{ nullptr };

		BaseEffect* m_pDiffusePhongEffect;
		BaseEffect* m_pTransparencyEffect;

		std::unique_ptr<Mesh> m_pMesh;
		std::unique_ptr<Mesh> m_pFireMesh;
		std::unique_ptr<Camera> m_pCamera;

		//Software rasterizer 
	private:

		SDL_Surface* m_pFrontBuffer{ nullptr };
		SDL_Surface* m_pBackBuffer{ nullptr };
		uint32_t* m_pBackBufferPixels{};
		float* m_pDepthBuffer{};


		void VertexTransformationFunction(const std::vector<VS_INPUT>& vertices_in, std::vector<Vertex_Out>& vertices_out) const;
		ColorRGB PixelShading(const Vertex_Out& v)const;

	public:
		ColorRGB m_uniformClearColor{ .1f, .1f, .1f };
		bool m_uniformClear{true};
		bool m_printFPS{false};
		bool m_showNormals{ false };
		bool m_showFire{ false };
		bool m_showDepth{ false };
		bool m_useSoftwareRasterizer{false};
		bool m_showBoundingBox{false};
		bool m_rotateMesh{false};
	};
}
