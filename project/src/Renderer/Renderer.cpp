#include "../pch.h"
#include "Renderer.h"

#include "../Mesh.h"
#include "../Utils.h"




namespace dae {
	inline bool TriangleTest(const Vertex_Out& v0, const Vertex_Out& v1, const Vertex_Out& v2, const Vector2& p, Vector3& outWeigths) {
		const Vector2 p0{ v0.position.x,v0.position.y };
		const Vector2 p1{ v1.position.x,v1.position.y };
		const Vector2 p2{ v2.position.x,v2.position.y };

		const float doubleTriangleArea{ Vector2::Cross(p1 - p0,p2 - p0) };
		outWeigths.z = { Vector2::Cross(p1 - p0,p - p0) / (doubleTriangleArea) };
		outWeigths.y = { Vector2::Cross(p0 - p2,p - p2) / (doubleTriangleArea) };
		outWeigths.x = { Vector2::Cross(p2 - p1,p - p1) / (doubleTriangleArea) };


		return (outWeigths.x >= 0.f && outWeigths.y >= 0.f && outWeigths.z >= 0.f);
	}
	Renderer::Renderer(SDL_Window* pWindow) :
		m_pWindow(pWindow)
	{
		//Initialize
		SDL_GetWindowSize(pWindow, &m_Width, &m_Height);

		//Create Buffers
		m_pFrontBuffer = SDL_GetWindowSurface(pWindow);
		m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
		m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;
		m_pDepthBuffer = new float[m_Width * m_Height];


		//Initialize DirectX pipeline
		const HRESULT result = InitializeDirectX();
		if (result == S_OK)
		{
			m_IsInitialized = true;
			std::cout << "DirectX is initialized and ready!\n";
		}
		else
		{
			std::cout << "DirectX initialization failed!\n";
		}
		const static std::string fireObjectFilePath("resources/fireFX.obj");

		const static std::string fireDiffuseTextureFilePath("resources/fireFX_diffuse.png");


		const static std::string vehicleObjectFilePath("resources/vehicle.obj");

		const static std::string vehicleDiffuseTextureFilePath("resources/vehicle_diffuse.png");
		const static std::string vehicleNormalTextureFilePath("resources/vehicle_normal.png");
		const static std::string vehicleGlossTextureFilePath("resources/vehicle_gloss.png");
		const static std::string vehicleSpecularTextureFilePath("resources/vehicle_specular.png");


		m_pFireDiffuseTexture = new Texture(m_pDevice, fireDiffuseTextureFilePath);

		m_pVehicleDiffuseTexture = new Texture(m_pDevice, vehicleDiffuseTextureFilePath);
		m_pVehicleNormalTexture = new Texture(m_pDevice, vehicleNormalTextureFilePath);
		m_pVehicleGlossTexture = new Texture(m_pDevice, vehicleGlossTextureFilePath);
		m_pVehicleSpecularTexture = new Texture(m_pDevice, vehicleSpecularTextureFilePath);


		m_pDiffusePhongEffect = new PhongDiffuseEffect(
			m_pDevice,
			m_pVehicleDiffuseTexture,
			m_pVehicleNormalTexture,
			m_pVehicleGlossTexture,
			m_pVehicleSpecularTexture
		);
		m_pTransparencyEffect = new TransparencyEffect(
			m_pDevice, m_pFireDiffuseTexture);

		m_pMesh = std::make_unique<Mesh>(m_pDevice,
			vehicleObjectFilePath,
			m_pDiffusePhongEffect,
			m_pVehicleDiffuseTexture,
			m_pVehicleNormalTexture,
			m_pVehicleGlossTexture,
			m_pVehicleSpecularTexture);

		m_pFireMesh = std::make_unique<Mesh>(m_pDevice,
			fireObjectFilePath,
			m_pTransparencyEffect,
			m_pFireDiffuseTexture
			);
		m_pCamera = std::make_unique<Camera>(float(m_Width)/m_Height);

	}

	Renderer::~Renderer()
	{
		if (m_pDeviceContext)
		{
			m_pDeviceContext->ClearState();
			m_pDeviceContext->Flush();
			m_pDeviceContext->Release();
		}

		if (m_pRenderTargetView)
		{
			m_pRenderTargetView->Release();
		}
		if (m_pRenderTargetBuffer)
		{
			m_pRenderTargetBuffer->Release();
		}
		if (m_pDepthStencilView)
		{
			m_pDepthStencilView->Release();
		}
		if (m_pDepthStencilBuffer)
		{
			m_pDepthStencilBuffer->Release();
		}
		if (m_pSwapChain)
		{
			m_pSwapChain->Release();
		}
		if (m_pDeviceContext)
		{
			m_pDeviceContext->Release();
		}
		if (m_pDevice)
		{
			m_pDevice->Release();
		}
		delete m_pTransparencyEffect;
		delete m_pDiffusePhongEffect;

		delete m_pFireDiffuseTexture;

		delete m_pVehicleDiffuseTexture;
		delete m_pVehicleNormalTexture;
		delete m_pVehicleGlossTexture;
		delete m_pVehicleSpecularTexture;

		//ReportLiveObjects();
	}

	void Renderer::Update(const Timer* pTimer)
	{
		{
			const uint8_t* keyBoardPtr = SDL_GetKeyboardState(nullptr);

			static bool f1Pressed = false;
			if (keyBoardPtr[SDL_SCANCODE_F1]) {
				if (!f1Pressed) {
					;
					m_useSoftwareRasterizer = !m_useSoftwareRasterizer;
					std::cout << "RasterizerMode : " << std::string(m_useSoftwareRasterizer ? "software" : "hardware") << std::endl;
				}
				f1Pressed = true;
			}
			else {
				f1Pressed = false;
			}
			static bool f2Pressed = false;
			if (keyBoardPtr[SDL_SCANCODE_F2]) {
				if (!f2Pressed) {
					m_rotateMesh = !m_rotateMesh;
					std::cout << "Rotating Mesh : " << std::string(m_rotateMesh ? "On" : "Off") << std::endl;
				}
				f2Pressed = true;
			}
			else {
				f2Pressed = false;
			}
			m_rotateMesh;
			static bool f8Pressed = false;
			if (keyBoardPtr[SDL_SCANCODE_F8]) {
				if (!f8Pressed) {
					m_showBoundingBox = !m_showBoundingBox;
					std::cout << "BoundingBox : " << std::string((m_showBoundingBox) ? "showing" : "hidden") << std::endl;
				}
				f8Pressed = true;
			}
			else {
				f8Pressed = false;
			}
			static bool f10Pressed = false;
			if (keyBoardPtr[SDL_SCANCODE_F10]) {
				if (!f10Pressed) {
					m_uniformClear = !m_uniformClear;
					std::cout << "Uniform Clear : " << std::string((m_uniformClear) ? "On" : "Off") << std::endl;
				}
				f10Pressed = true;
			}
			else {
				f10Pressed = false;
			}
			static bool f11Pressed = false;
			if (keyBoardPtr[SDL_SCANCODE_F11]) {
				if (!f11Pressed) {
					m_printFPS = !m_printFPS;
					std::cout << "Print FPS : " << std::string((m_printFPS) ? "On" : "Off") << std::endl;
				}
				f11Pressed = true;
			}
			else {
				f11Pressed = false;
			}
			static bool f6Pressed = false;
			if (keyBoardPtr[SDL_SCANCODE_F6]) {
				if (!f6Pressed) {
					m_showNormals = !m_showNormals;
					std::cout << "Showing Normals : " << std::string((m_showNormals) ? "On" : "Off") << std::endl;
				}
				f6Pressed = true;
			}
			else {
				f6Pressed = false;
			}
			static bool f3Pressed = false;
			if (keyBoardPtr[SDL_SCANCODE_F3]) {
				if (!f3Pressed) {
					m_showFire = !m_showFire;
					std::cout << "Showing Fire : " << std::string((m_showFire) ? "On" : "Off") << std::endl;
				}
				f3Pressed = true;
			}
			else {
				f3Pressed = false;
			}
			static bool f7Pressed = false;
			if (keyBoardPtr[SDL_SCANCODE_F7]) {
				if (!f7Pressed) {
					m_showDepth = !m_showDepth;
					std::cout << "Showing Depth : " << std::string((m_showDepth) ? "On" : "Off") << std::endl;
				}
				f7Pressed = true;
			}
			else {
				f7Pressed = false;
			}
		}


		if (m_rotateMesh) {
			m_pMesh->Rotate(Vector3{0,pTimer->GetElapsed(),0});
		}

		m_pCamera->Update(pTimer);
		///m_pMesh->Update(pTimer);

		auto viewMatrix = m_pCamera->GetViewMatrix();
		auto projectionMatrix = m_pCamera->GetProjectionMatrix();

		m_pMesh->GetEffectPtr()->SetWorldViewProjectionMatrix(viewMatrix * projectionMatrix);
		m_pMesh->GetEffectPtr()->SetWorldMatrix(m_pMesh->GetWorldMatrix());
		m_pMesh->GetEffectPtr()->SetCameraPosition(m_pCamera->GetPosition());
		m_pFireMesh->GetEffectPtr()->SetWorldViewProjectionMatrix(viewMatrix * projectionMatrix);
		m_pFireMesh->GetEffectPtr()->SetWorldMatrix(m_pFireMesh->GetWorldMatrix());
		m_pFireMesh->GetEffectPtr()->SetCameraPosition(m_pCamera->GetPosition());
	}


	void Renderer::Render() const
	{
		if (m_useSoftwareRasterizer) {
			SoftwareRender();
		}
		else {
			HardwareRender();
		}

	}

	void Renderer::SoftwareRender() const
	{
		SDL_LockSurface(m_pBackBuffer);
		ColorRGB backgroundColor{ .39f, .39f, .39f };
		if (m_uniformClear) {
			backgroundColor = m_uniformClearColor;
		}

		std::fill_n(m_pDepthBuffer, m_Width * m_Height, std::numeric_limits<float>::max());
		std::fill_n(m_pBackBufferPixels, m_Width * m_Height, SDL_MapRGB(m_pBackBuffer->format,
			static_cast<uint8_t>(backgroundColor.r * 255),
			static_cast<uint8_t>(backgroundColor.g * 255),
			static_cast<uint8_t>(backgroundColor.b * 255)));



		VertexTransformationFunction(m_pMesh->m_vertices, m_pMesh->m_vertices_out);
		//transforming projected verts to screenspace

		if (m_pMesh->m_vertices_out.size() < 1)return;


		for (int i{ 0 }; i < m_pMesh->m_vertices_out.size(); i++) {
			m_pMesh->m_vertices_out[i].position.x = ((m_pMesh->m_vertices_out[i].position.x + 1) / 2.0f) * m_Width;
			m_pMesh->m_vertices_out[i].position.y = ((1 - m_pMesh->m_vertices_out[i].position.y) / 2.0f) * m_Height;

		}
		for (int i{}; i < m_pMesh->m_indices.size(); i += 3) {

			const Vertex_Out& v0{ m_pMesh->m_vertices_out[m_pMesh->m_indices[i + 0]] };
			const Vertex_Out& v1{ m_pMesh->m_vertices_out[m_pMesh->m_indices[i + 1]] };
			const Vertex_Out& v2{ m_pMesh->m_vertices_out[m_pMesh->m_indices[i + 2]] };

			if ((Vector3::Dot(v1.normal, v1.viewDirection) < 0 && Vector3::Dot(v2.normal, v2.viewDirection) < 0 && Vector3::Dot(v0.normal, v0.viewDirection) < 0)) {
				continue;
			}

			float xmin_boundingBox{ std::min(v0.position.x,std::min(v1.position.x,v2.position.x)) };
			float xmax_boundingBox{ std::max(v0.position.x,std::max(v1.position.x,v2.position.x)) };
			float ymin_boundingBox{ std::min(v0.position.y,std::min(v1.position.y,v2.position.y)) };
			float ymax_boundingBox{ std::max(v0.position.y,std::max(v1.position.y,v2.position.y)) };



			//Clamping bounding box to screen size
			xmin_boundingBox = std::min(m_Width - 1.f, std::max(0.f, xmin_boundingBox));
			xmax_boundingBox = std::min(m_Width - 1.f, std::max(0.f, xmax_boundingBox));
			ymin_boundingBox = std::min(m_Height - 1.f, std::max(0.f, ymin_boundingBox));
			ymax_boundingBox = std::min(m_Height - 1.f, std::max(0.f, ymax_boundingBox));


			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// !!!!!!Make sure to calculate Bounding box around triangle and not the entire mesh to avoid doing triangle test with a bounding box way bigger than the triagnle itself
			//!!!!!!!!!!!!!!!
			for (int px{ (int)xmin_boundingBox }; px <= (int)xmax_boundingBox; ++px)
			{
				for (int py{ (int)ymin_boundingBox }; py <= (int)ymax_boundingBox; ++py)
				{
					ColorRGB finalColor{ };

					const Vector2 p{ float(px),float(py) };
					

					Vector3 weigths{};
					if (TriangleTest(v0, v1, v2, p, weigths)) {


						const float depth{ 1.f / ((weigths.x / (v0.position.w)) + (weigths.y / (v1.position.w)) + (weigths.z / (v2.position.w))) };
						const float clippingDepth{ 1.f / ((weigths.x / (v0.position.z)) + (weigths.y / (v1.position.z)) + (weigths.z / (v2.position.z))) };

						//depth_test
						if (m_pDepthBuffer[px + py * m_Width] > depth && (clippingDepth >= 0 && clippingDepth <= 1)) {
							m_pDepthBuffer[px + py * m_Width] = depth;

							//Vertex_Out vOut{
							//	Vector4{(weigths.x * v0.position / (v0.position.w) + weigths.y * v1.position / (v1.position.w) + weigths.z * v2.position / (v2.position.w)) * depth,1.0f},
							//	ColorRGB{weigths.x,weigths.y,weigths.z},//v0.color * weigths.x + v1.color * weigths.y + v2.color * weigths.z,
							//	(weigths.x * v0.uv / (v0.position.w) + weigths.y * v1.uv / (v1.position.w) + weigths.z * v2.uv / (v2.position.w)) * depth,
							//	((weigths.x * v0.normal / (v0.position.w) + weigths.y * v1.normal / (v1.position.w) + weigths.z * v2.normal / (v2.position.w))* depth).Normalized(),//weigths.x * v0.normal + weigths.y * v1.normal + weigths.z * v2.normal,
							//	((weigths.x * v0.tangent / (v0.position.w) + weigths.y * v1.tangent / (v1.position.w) + weigths.z * v2.tangent / (v2.position.w))* depth).Normalized(),//weigths.x * v0.tangent + weigths.y * v1.tangent + weigths.z * v2.tangent,
							//	((weigths.x * v0.viewDirection / (v0.position.w) + weigths.y * v1.viewDirection / (v1.position.w) + weigths.z * v2.viewDirection / (v2.position.w))* depth).Normalized(),
							//};
							Vertex_Out vOut{
								// Position: perspective-correct
								Vector4{(weigths.x * v0.position / (v0.position.w) + weigths.y * v1.position / (v1.position.w) + weigths.z * v2.position / (v2.position.w)) * depth, 1.0f},
								// Color: barycentric
								ColorRGB{weigths.x, weigths.y, weigths.z},
								// UV: perspective-correct
								(weigths.x * v0.uv / (v0.position.w) + weigths.y * v1.uv / (v1.position.w) + weigths.z * v2.uv / (v2.position.w)) * depth,
								// Normal: linear
								(weigths.x * v0.normal + weigths.y * v1.normal + weigths.z * v2.normal).Normalized(),
								// Tangent: linear
								(weigths.x * v0.tangent + weigths.y * v1.tangent + weigths.z * v2.tangent).Normalized(),
								// ViewDirection: linear
								(weigths.x * v0.viewDirection + weigths.y * v1.viewDirection + weigths.z * v2.viewDirection).Normalized(),
							};
							finalColor = PixelShading(vOut);

							finalColor.MaxToOne();

							m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
								static_cast<uint8_t>(finalColor.r * 255),
								static_cast<uint8_t>(finalColor.g * 255),
								static_cast<uint8_t>(finalColor.b * 255));
						}
					}
					if (m_showBoundingBox) {
						const float BorderPixelWidth{2};
						if (px <= xmin_boundingBox + BorderPixelWidth || 
							py <= ymin_boundingBox + BorderPixelWidth || 
							px >= xmax_boundingBox - BorderPixelWidth || 
							py >= ymax_boundingBox - BorderPixelWidth) {
							finalColor = ColorRGB{ 1.0f,0.f,0.f };
							finalColor.MaxToOne();
					
							m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
								static_cast<uint8_t>(finalColor.r * 255),
								static_cast<uint8_t>(finalColor.g * 255),
								static_cast<uint8_t>(finalColor.b * 255));
						}
					}
				}
			}
		}
		//RENDER LOGIC


		//@END
		//Update SDL Surface
		SDL_UnlockSurface(m_pBackBuffer);
		SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
		SDL_UpdateWindowSurface(m_pWindow);
	}

	void Renderer::HardwareRender() const
	{
		if (!m_IsInitialized)
			return;

		float color[4] = { .39f, .59f, .93f , 1.f };
		if (m_uniformClear) {
			color[0] = m_uniformClearColor.r;
			color[1] = m_uniformClearColor.g;
			color[2] = m_uniformClearColor.b;
		}


		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

		// 2. SET PIPELINE + INVOKE DRAW CALLS (= RENDER)

		m_pMesh->Render(m_pDeviceContext, m_pCamera->GetSamplingTechniqueIndex());
		if(m_showFire)
			m_pFireMesh->Render(m_pDeviceContext, m_pCamera->GetSamplingTechniqueIndex());


		// 3. PRESENT BACKBUFFER (SWAP)
		m_pSwapChain->Present(0, 0);
	}

	HRESULT Renderer::InitializeDirectX()
	{


		//CONFIGURING AND CREATING DEVICE AND DEVICE CONTEXT
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

		uint32_t createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		HRESULT result = D3D11CreateDevice(nullptr,D3D_DRIVER_TYPE_HARDWARE,0,createDeviceFlags,&featureLevel,1,D3D11_SDK_VERSION,&m_pDevice,nullptr,&m_pDeviceContext);
		if (FAILED(result)) {
			__debugbreak();
			return result;
		}

		//CREATING SWAPCHAIN-----------------------------------------------------

		//Create a DXGI Factory to Create a custom SWAPCHAIN to fit your hardware
		IDXGIFactory1* dxgiFactoryPtr{};
		result = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactoryPtr));
		if (FAILED(result)) {
			__debugbreak();
			return result;
		}


		DXGI_SWAP_CHAIN_DESC swapChainDescriptor{};
		swapChainDescriptor.BufferDesc.Width = m_Width;
		swapChainDescriptor.BufferDesc.Height = m_Height;
		swapChainDescriptor.BufferDesc.RefreshRate.Numerator = 1;
		swapChainDescriptor.BufferDesc.RefreshRate.Denominator = 60;
		swapChainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDescriptor.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDescriptor.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDescriptor.SampleDesc.Count = 1;
		swapChainDescriptor.SampleDesc.Quality = 0;
		swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDescriptor.BufferCount = 1;
		swapChainDescriptor.Windowed = true;
		swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDescriptor.Flags = 0;
		//---------------------------------------------------------------



		// Retrieving the window handle to attach the swapchain to the window
		SDL_SysWMinfo sysWMinfo{};
		SDL_GetVersion(&sysWMinfo.version);
		SDL_GetWindowWMInfo(m_pWindow, &sysWMinfo);

		swapChainDescriptor.OutputWindow = sysWMinfo.info.win.window;


		// Creating Texture 2d To use as depht stencil
		D3D11_TEXTURE2D_DESC depthStencilDesc{};
		depthStencilDesc.Width = m_Width;
		depthStencilDesc.Height = m_Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		// Making a resource view to use texture 2d as a depthStencilBuffer 
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		depthStencilViewDesc.Format = depthStencilDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// After which we create both the resource and matching resource view.
		result = m_pDevice->CreateTexture2D(&depthStencilDesc,  nullptr, &m_pDepthStencilBuffer);
		if (FAILED(result)) {
			__debugbreak();
			return result;
		}


		result = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
		if (FAILED(result)) {
			__debugbreak();
			return result;
		}


		result = dxgiFactoryPtr->CreateSwapChain(m_pDevice,&swapChainDescriptor,&m_pSwapChain);
		dxgiFactoryPtr->Release();
		if (FAILED(result)) {
			__debugbreak();
			return result;
		}
		// 4. Create RenderTarget (RT) & RenderTargetView (RTV)
// =====

// Resource
		result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pRenderTargetBuffer));
		if (FAILED(result)) {
			__debugbreak();
			return result;
		}
		// View
		///---------------------------------------------------------------------------------
		// A View is just an interpretation of data just like casting raw data to a datatype
		///---------------------------------------------------------------------------------

		result = m_pDevice->CreateRenderTargetView(m_pRenderTargetBuffer, nullptr, &m_pRenderTargetView);
		if (FAILED(result)) {
			__debugbreak();
			return result;
		}
		// 5. Bind RTV & DSV to Output Merger Stage
		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

		//6. Set Viewport
		//=====
		D3D11_VIEWPORT viewport{};
		viewport.Width = static_cast<float>(m_Width);
		viewport.Height = static_cast<float>(m_Height);
		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;
		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;
		m_pDeviceContext->RSSetViewports(1, &viewport);

		return S_OK;
	}
}


void dae::Renderer::VertexTransformationFunction(const std::vector<VS_INPUT>& vertices_in, std::vector<Vertex_Out>& vertices_out) const
{
	const bool orthographic{ false };
	vertices_out.clear();


	Matrix worldMatrix{ m_pMesh->GetWorldMatrix()};
	// order in which matrices are applied is from left to rigth 
	auto m = m_pCamera->GetViewMatrix() * m_pCamera->GetProjectionMatrix();

	for (const VS_INPUT& p : vertices_in) {
		Vector4 wp{ worldMatrix.TransformPoint(Vector4{ p.position[0],p.position[1],p.position[2], 1.0f})};
		Vector4 np{ m.TransformPoint(wp) };

		const Vector3 newNormal{ worldMatrix.TransformVector(Vector3{ p.normal[0],p.normal[1],p.normal[2]}) };
		const Vector3 newTangent{ worldMatrix.TransformVector(Vector3{ p.tangent[0],p.tangent[1],p.tangent[2]}) };
		if (!orthographic) {
			np.x /= np.w;
			np.y /= np.w;
			np.z /= np.w;
		}
		Vertex_Out nv{ np ,
			ColorRGB{p.color[0],p.color[1],p.color[2]},
			Vector2{ p.uv[0],p.uv[1]},
			newNormal,
			newTangent,
			(m_pCamera->GetPosition() - Vector3{wp.x,wp.y,wp.z}).Normalized() };
		vertices_out.emplace_back(nv);
	}
}

dae::ColorRGB dae::Renderer::PixelShading(const Vertex_Out& v)const
{
	const Vector3 ligthDirection{ .577f,-.577f,.577f };

	Vector3 normal{ v.normal };
	// Sampling normal vector
	if (const Texture* normalMap = m_pMesh->GetNormalTexture()) {
		ColorRGB sampledNormal = m_pMesh->GetNormalTexture()->Sample(v.uv);
		Vector3 processedNormal = 2.0f * Vector3{ sampledNormal.r,sampledNormal.g ,sampledNormal.b } - Vector3{ 1.0f,1.0f,1.0f };
		Matrix ONB = Matrix(v.tangent, Vector3::Cross(v.normal, v.tangent).Normalized(), v.normal, Vector3{});
		normal = (ONB.TransformVector(processedNormal)).Normalized();
	}


	ColorRGB lambertDiffuse{1.0f,1.0f,1.0f};
	if (const Texture* diffuseTexture = m_pMesh->GetDiffuseTexture()) {
		const float diffuseReflectionCoef{ 7.0f };
		lambertDiffuse =  diffuseReflectionCoef* diffuseTexture->Sample(v.uv) / PI ;
	}

	float phongSpecularReflection{};
	if (m_pMesh->GetGlossTexture() && m_pMesh->GetSpecularTexture()) {
		const float shininess{ 25.0f };
		ColorRGB glossSample = m_pMesh->GetGlossTexture()->Sample(v.uv);
		ColorRGB specularSample = m_pMesh->GetSpecularTexture()->Sample(v.uv);

		Vector3 reflectedVector{ Vector3::Reflect(ligthDirection,normal).Normalized() };


		float cosAlpha = std::abs(Vector3::Dot(reflectedVector, v.viewDirection));
		phongSpecularReflection = (specularSample.r * std::powf(cosAlpha, glossSample.r * shininess));
		if (isnan(phongSpecularReflection)) {
			phongSpecularReflection = 0.0f;
		}
	}
	
	const float observedArea{ abs(Vector3::Dot(-ligthDirection.Normalized(), normal)) };

	if (m_showDepth) {
		return ColorRGB{ v.position.w,v.position.w,v.position.w};
	}
	if (m_showNormals) {
		return ColorRGB{ (normal.x + 1.0f) *0.5f, (normal.y + 1.0f) * 0.5f, (normal.z + 1.0f) * 0.5f };

	}
	switch (m_pCamera->renderingmode) {
	case Camera::RenderingMode::Specular:
		return  ColorRGB{ phongSpecularReflection ,phongSpecularReflection ,phongSpecularReflection };
		break;
	case Camera::RenderingMode::ObservedArea:

		//return ColorRGB{ (reflectedVector.x + 1.0f) *0.5f, (reflectedVector.y + 1.0f) * 0.5f, (reflectedVector.z + 1.0f) * 0.5f };
		//return ColorRGB{ cosAlpha, cosAlpha, cosAlpha };
		//return ColorRGB{ (normal.x + 1.0f) *0.5f, (normal.y + 1.0f) * 0.5f, (normal.z + 1.0f) * 0.5f };
		//return ColorRGB{ v.tangent.x,v.tangent.y,v.normal.z };
		//return ColorRGB{ v.viewDirection.x *0.5f +0.5f,v.viewDirection.y * 0.5f + 0.5f,v.viewDirection.z * 0.5f + 0.5f };

		return ColorRGB{ observedArea, observedArea, observedArea };
		break;
	case Camera::RenderingMode::Diffuse:
		return m_pMesh->GetDiffuseTexture()->Sample(v.uv);
		break;
	case Camera::RenderingMode::Combined:
		return observedArea * lambertDiffuse + ColorRGB{ phongSpecularReflection ,phongSpecularReflection ,phongSpecularReflection };
		break;

	}


	return observedArea * lambertDiffuse + ColorRGB{ phongSpecularReflection ,phongSpecularReflection ,phongSpecularReflection };

}
