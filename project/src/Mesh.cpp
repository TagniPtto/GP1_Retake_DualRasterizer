#include "Mesh.h"
#include "utils.h"
#include "Math/Math.h"

#include <iostream>




Mesh::Mesh(const std::string& objectFilePath)
{
	dae::Utils::ParseOBJ(objectFilePath,m_vertices,m_indices,false);
}

void Mesh::Rotate(const dae::Vector3& rotation)
{
	m_worldRotation += rotation;
}

void Mesh::Translate(const dae::Vector3& translation)
{
	m_worldPosition += translation;
}
void Mesh::Scale(const dae::Vector3& scale)
{
	m_worldScale.x *= scale.x;
	m_worldScale.y *= scale.y;
	m_worldScale.z *= scale.z;
}

void Mesh::SetRotation(const dae::Vector3& rotation)
{ 
	m_worldRotation = rotation;
}

void Mesh::SetPosition(const dae::Vector3& position)
{
	m_worldPosition = position;
}

void Mesh::SetScale(const dae::Vector3& scale)
{
	m_worldScale = scale;
}

dae::Matrix Mesh::GetWorldMatrix() const
{
	return dae::Matrix::CreateTranslation(m_worldPosition) * dae::Matrix::CreateRotation(m_worldRotation) * dae::Matrix::CreateScale(m_worldScale);
}

Material* Mesh::GetMaterial() const
{
	return m_pMaterial;
}



