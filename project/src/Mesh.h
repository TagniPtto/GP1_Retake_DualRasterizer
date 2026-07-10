
#pragma once
#include "d3d11.h"

#include "DataTypes.h"
#include "Texture.h"
#include "BaseEffect.h"
#include "Math/Math.h"

#include <vector>

class Mesh {
public:
	Mesh(ID3D11Device* pDevice,
		std::string objectFilePath,
		BaseEffect* effect,
		Texture* pDiffuseTexture = nullptr,
		Texture* pNormalTexture = nullptr,
		Texture* pGlossTexture = nullptr,
		Texture* pSpecularTexture = nullptr);
	~Mesh();
	void Rotate(const dae::Vector3& rotation);
	void Translate(const dae::Vector3& translation);

	void SetRotation(const dae::Vector3& rotation);
	void SetPosition(const dae::Vector3& position);
	void Scale(const dae::Vector3& scale);


	void Render(ID3D11DeviceContext* pDeviceContext , int techniqueIndex);


	BaseEffect* GetEffectPtr();
	dae::Matrix GetWorldMatrix();
	Texture* GetDiffuseTexture();
	Texture* GetNormalTexture();
	Texture* GetGlossTexture();
	Texture* GetSpecularTexture();
public:

	std::vector<VS_INPUT> m_vertices{
	{{-3.0f,3.0f,2.0f},{1.0f,0.0f,0.0f},{0,0}},
	{{3.f,3.f,2.0f},{0.0f,1.0f,0.0f},{1,0}},
	{{3.f,-3.f,2.0f},{0.0f,0.0f,1.0f},{1,1}},
	{{-3.0f,-3.0f,2.0f},{1.0f,1.0f,0.0f},{0,1}},
	};

	std::vector<uint32_t> m_indices{0,1,2,2,3,0};

	PrimitiveTopology m_triangleTopology{ PrimitiveTopology::TriangleList};
	std::vector<Vertex_Out> m_vertices_out{};
private:
	ID3D11Buffer* m_pVertexBuffer{nullptr};
	ID3D11Buffer* m_pIndexBuffer{nullptr};

	ID3D11Texture2D* m_pTextureBuffer{nullptr};
	Texture* m_pDiffuseTexture{ nullptr };
	Texture* m_pNormalTexture{ nullptr };
	Texture* m_pGlossTexture{ nullptr };
	Texture* m_pSpecularTexture{ nullptr };
	BaseEffect* m_pEffect{ nullptr };

	dae::Vector3 m_worldPosition{0.0f,0.0f,0.0f};
	dae::Vector3 m_worldRotation{};
	dae::Vector3 m_worldScale{1.0f,1.0f ,1.0f };
};