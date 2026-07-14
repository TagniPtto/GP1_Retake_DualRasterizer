
#pragma once

#include "DataTypes.h"
#include "Math/Math.h"
#include "Material.h"

#include <vector>

class Mesh final{
public:
	Mesh(const std::string& objectFilePath);
	~Mesh() = default;
	void Rotate(const dae::Vector3& rotation);
	void Translate(const dae::Vector3& translation);
	void Scale(const dae::Vector3& scale);

	void SetRotation(const dae::Vector3& rotation);
	void SetPosition(const dae::Vector3& position);
	void SetScale(const dae::Vector3& scale);


	dae::Matrix GetWorldMatrix() const;
	Material* GetMaterial() const;

	void Render(ID3D11DeviceContext* pDeviceContext , int techniqueIndex) ;


public:
	PrimitiveTopology m_triangleTopology{ PrimitiveTopology::TriangleList };


	std::vector<VS_INPUT> m_vertices{
	{{-3.0f,3.0f,	2.0f}	,	{1.0f,	0.0f,	0.0f}	,	{0,	0}},
	{{3.f,	3.f,	2.0f}	,	{0.0f,	1.0f,	0.0f}	,	{1,	0}},
	{{3.f,	-3.f,	2.0f}	,	{0.0f,	0.0f,	1.0f}	,	{1,	1}},
	{{-3.0f,-3.0f,	2.0f}	,	{1.0f,	1.0f,	0.0f}	,	{0,	1}},
	};
	std::vector<uint32_t> m_indices{0,1,2,2,3,0};


	std::vector<VS_OUTPUT> m_vertices_out{};
private:

	Material* m_pMaterial;

	dae::Vector3 m_worldPosition{0.0f,0.0f,0.0f};
	dae::Vector3 m_worldRotation{};
	dae::Vector3 m_worldScale{1.0f,1.0f ,1.0f };
};