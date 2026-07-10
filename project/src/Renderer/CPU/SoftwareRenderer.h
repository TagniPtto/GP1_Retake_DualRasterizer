#pragma once
#include "../IRenderer.h"

namespace dae {
	class SoftwareRenderer final : public IRenderer {
	public:
		virtual ~SoftwareRenderer() override;
		void Render() const override;
	};
}
