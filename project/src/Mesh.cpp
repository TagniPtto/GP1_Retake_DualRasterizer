#include "Mesh.h"

#include <iostream>


#include "utils.h"


Mesh::Mesh(ID3D11Device* pDevice,
		std::string objectFilePath,
		BaseEffect* effect,
		Texture* pDiffuseTexture,
		Texture* pNormalTexture,
		Texture* pGlossTexture,
		Texture* pSpecularTexture
	):
	m_pEffect(effect), 
	m_pDiffuseTexture(pDiffuseTexture),
	m_pNormalTexture(pNormalTexture),
	m_pGlossTexture(pGlossTexture),
	m_pSpecularTexture(pSpecularTexture)
{
	

	dae::Utils::ParseOBJ(objectFilePath,m_vertices,m_indices,false);


	//describing the vertex buffer layout ---------------------------------------------------------
	D3D11_BUFFER_DESC bufferDescription{};
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.ByteWidth = sizeof(VS_INPUT) * static_cast<uint32_t>(m_vertices.size());
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescription.CPUAccessFlags = 0;
	bufferDescription.MiscFlags = 0;
	//---------------------------------------------------------------------------------------------


	///Creating the vertex buffer using the descriptino we just provided----------------------------
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = m_vertices.data(); 
	HRESULT result = pDevice->CreateBuffer(&bufferDescription, &initData, &m_pVertexBuffer);
	///---------------------------------------------------------------------------------------------


	if (FAILED(result)) {
		std::cout << "failed to create Vertex buffer\n";
		return;
	}

	//describing the index buffer layout ---------------------------------------------------------
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.ByteWidth = sizeof(uint32_t) * static_cast<uint32_t>(m_indices.size());
	bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDescription.CPUAccessFlags = 0;
	bufferDescription.MiscFlags = 0;
	//---------------------------------------------------------------------------------------------


	/// Creating the index buffer using the buffer description we just provided--------------------
	initData.pSysMem = m_indices.data();
	result = pDevice->CreateBuffer(&bufferDescription,&initData, &m_pIndexBuffer);
	///---------------------------------------------------------------------------------------------

}

Mesh::~Mesh()
{
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer  = nullptr;

	delete m_pEffect;
	m_pEffect = nullptr;

}

void Mesh::Rotate(const dae::Vector3& rotation)
{
	m_worldRotation += rotation;
}

void Mesh::Translate(const dae::Vector3& translation)
{
	m_worldPosition += translation;
}

void Mesh::SetRotation(const dae::Vector3& rotation)
{ 
	m_worldRotation = rotation;
}

void Mesh::SetPosition(const dae::Vector3& position)
{
	m_worldPosition = position;
}


void Mesh::Scale(const dae::Vector3& scale)
{
	m_worldScale = scale;
}

void Mesh::Render(ID3D11DeviceContext* pDeviceContext , int techniqueIndex)
{
	//IA input assembly stages
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->IASetInputLayout(m_pEffect->GetInputLayoutPtr());

	constexpr UINT stride = sizeof(VS_INPUT);
	constexpr UINT location = 0;
	constexpr UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(location,1,&m_pVertexBuffer,&stride, &offset);
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer,DXGI_FORMAT_R32_UINT,offset);

	D3DX11_TECHNIQUE_DESC techniqueDescription{};

	const HRESULT result = m_pEffect->GetEffectTechniquePtr(techniqueIndex)->GetDesc(&techniqueDescription);
	if (FAILED(result)) {
		std::cout << "failed to Get TechniqueDescription" << std::endl;
	}
	for (UINT i{}; i < techniqueDescription.Passes; i++) {

		m_pEffect->GetEffectTechniquePtr(techniqueIndex)->GetPassByIndex(i)->Apply(0,pDeviceContext);
		pDeviceContext->DrawIndexed(m_indices.size(),0, 0);

	}
}

BaseEffect* Mesh::GetEffectPtr()
{
	return m_pEffect;
}

Matrix Mesh::GetWorldMatrix()
{
	return dae::Matrix::CreateTranslation(m_worldPosition) * dae::Matrix::CreateRotation(m_worldRotation) * dae::Matrix::CreateScale(m_worldScale);
}

Texture* Mesh::GetDiffuseTexture()
{
	return m_pDiffuseTexture;
}

Texture* Mesh::GetNormalTexture()
{
	return m_pNormalTexture;
}

Texture* Mesh::GetGlossTexture()
{
	return m_pGlossTexture;
}

Texture* Mesh::GetSpecularTexture()
{
	return m_pSpecularTexture;
}

