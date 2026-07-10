#include "SoftwareRenderer.h"

#include "../../DataTypes.h"
#include "../../Math/Math.h"

namespace dae 
{
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
}
