#pragma once
#include "../IRenderer.h"

namespace dae {
	class SoftwareRenderer final : public IRenderer {
	public:
		virtual ~SoftwareRenderer() override;
		virtual void Render() const override;
		virtual void Update(const Timer* pTimer) override;
	private:
		void VertexTransformationFunction(const std::vector<VS_INPUT>& vertices_in, std::vector<Vertex_Out>& vertices_out) const;
		ColorRGB PixelShading(const Vertex_Out& v)const;

	};
}
