#include "SoftwareRenderer.h"

#include "../../DataTypes.h"
#include "../../Math/Math.h"

namespace dae 
{
	inline bool TriangleTest(const VS_OUTPUT& v0, const VS_OUTPUT& v1, const VS_OUTPUT& v2, const Vector2& p, Vector3& outWeigths) {
		const Vector2 p0{ v0.position.x,v0.position.y };
		const Vector2 p1{ v1.position.x,v1.position.y };
		const Vector2 p2{ v2.position.x,v2.position.y };

		const float doubleTriangleArea{ Vector2::Cross(p1 - p0,p2 - p0) };
		outWeigths.z = { Vector2::Cross(p1 - p0,p - p0) / (doubleTriangleArea) };
		outWeigths.y = { Vector2::Cross(p0 - p2,p - p2) / (doubleTriangleArea) };
		outWeigths.x = { Vector2::Cross(p2 - p1,p - p1) / (doubleTriangleArea) };


		return (outWeigths.x >= 0.f && outWeigths.y >= 0.f && outWeigths.z >= 0.f);
	}
	void SoftwareRenderer::Render() const
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

			const VS_OUTPUT& v0{ m_pMesh->m_vertices_out[m_pMesh->m_indices[i + 0]] };
			const VS_OUTPUT& v1{ m_pMesh->m_vertices_out[m_pMesh->m_indices[i + 1]] };
			const VS_OUTPUT& v2{ m_pMesh->m_vertices_out[m_pMesh->m_indices[i + 2]] };

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

						//Formula for corrective depth can be derived from some basic math see ->
						//https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/visibility-problem-depth-buffer-depth-interpolation.html


						const float depth{ 1.f / ((weigths.x / (v0.position.w)) + (weigths.y / (v1.position.w)) + (weigths.z / (v2.position.w))) };
						const float clippingDepth{ 1.f / ((weigths.x / (v0.position.z)) + (weigths.y / (v1.position.z)) + (weigths.z / (v2.position.z))) };

						//depth_test
						if (m_pDepthBuffer[px + py * m_Width] > depth && (clippingDepth >= 0 && clippingDepth <= 1)) {
							m_pDepthBuffer[px + py * m_Width] = depth;

							VS_OUTPUT vOut{
								Vector4{(weigths.x * v0.position / (v0.position.w) + weigths.y * v1.position / (v1.position.w) + weigths.z * v2.position / (v2.position.w)) * depth,1.0f},
								ColorRGB{weigths.x,weigths.y,weigths.z},
								(weigths.x * v0.uv / (v0.position.w) + weigths.y * v1.uv / (v1.position.w) + weigths.z * v2.uv / (v2.position.w)) * depth,
								((weigths.x * v0.normal / (v0.position.w) + weigths.y * v1.normal / (v1.position.w) + weigths.z * v2.normal / (v2.position.w)) * depth).Normalized(),
								((weigths.x * v0.tangent / (v0.position.w) + weigths.y * v1.tangent / (v1.position.w) + weigths.z * v2.tangent / (v2.position.w)) * depth).Normalized(),
								((weigths.x * v0.viewDirection / (v0.position.w) + weigths.y * v1.viewDirection / (v1.position.w) + weigths.z * v2.viewDirection / (v2.position.w)) * depth).Normalized(),
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
						const float BorderPixelWidth{ 2 };
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
	void SoftwareRenderer::Update(const Timer * pTimer)
	{

	}
	void SoftwareRenderer::VertexTransformationFunction(const std::vector<VS_INPUT>& vertices_in, std::vector<VS_OUTPUT>& vertices_out) const
	{
		const bool orthographic{ false };
		vertices_out.clear();


		Matrix worldMatrix{ m_pMesh->GetWorldMatrix() };
		// order in which matrices are applied is from left to rigth 
		auto m = m_pCamera->GetViewMatrix() * m_pCamera->GetProjectionMatrix();

		for (const VS_INPUT& p : vertices_in) {
			Vector4 wp{ worldMatrix.TransformPoint(Vector4{ p.position[0],p.position[1],p.position[2], 1.0f}) };
			Vector4 np{ m.TransformPoint(wp) };

			const Vector3 newNormal{ worldMatrix.TransformVector(Vector3{ p.normal[0],p.normal[1],p.normal[2]}) };
			const Vector3 newTangent{ worldMatrix.TransformVector(Vector3{ p.tangent[0],p.tangent[1],p.tangent[2]}) };
			if (!orthographic) {
				np.x /= np.w;
				np.y /= np.w;
				np.z /= np.w;
			}
			VS_OUTPUT nv{ np ,
				ColorRGB{p.color[0],p.color[1],p.color[2]},
				Vector2{ p.uv[0],p.uv[1]},
				newNormal,
				newTangent,
				(m_pCamera->GetPosition() - Vector3{wp.x,wp.y,wp.z}).Normalized() };
			vertices_out.emplace_back(nv);
		}
	}
	ColorRGB SoftwareRenderer::PixelShading(const VS_OUTPUT& v) const
	{

		const Vector3 ligthDirection{ .577f,-.577f,.577f };
		const Material* material = m_pmesh->GetMaterial();

		Vector3 normal{ v.normal };
		// Sampling normal vector
		if (const Texture* normalMap =  material->GetNormalTexture()) {
			ColorRGB sampledNormal = m_pMesh->GetNormalTexture()->Sample(v.uv);
			Vector3 processedNormal = 2.0f * Vector3{ sampledNormal.r,sampledNormal.g ,sampledNormal.b } - Vector3{ 1.0f,1.0f,1.0f };
			Matrix ONB = Matrix(v.tangent, Vector3::Cross(v.normal, v.tangent).Normalized(), v.normal, Vector3{});
			normal = (ONB.TransformVector(processedNormal)).Normalized();
		}


		ColorRGB lambertDiffuse{ 1.0f,1.0f,1.0f };
		if (const Texture* diffuseTexture = m_pMesh->GetDiffuseTexture()) {
			const float diffuseReflectionCoef{ 7.0f };
			lambertDiffuse = diffuseReflectionCoef * diffuseTexture->Sample(v.uv) / PI;
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
			return ColorRGB{ v.position.w,v.position.w,v.position.w };
		}
		if (m_showNormals) {
			return ColorRGB{ (normal.x + 1.0f) * 0.5f, (normal.y + 1.0f) * 0.5f, (normal.z + 1.0f) * 0.5f };

		}
		switch (m_pCamera->renderingmode) {
		case Camera::RenderingMode::Specular:
			return  ColorRGB{ phongSpecularReflection ,phongSpecularReflection ,phongSpecularReflection };
			break;
		case Camera::RenderingMode::ObservedArea:

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
}
