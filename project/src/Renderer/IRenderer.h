#pragma once
#include "pch.h"


namespace dae {
	class IRenderer {
	public:
		virtual ~IRenderer() = default;

		virtual void Render() const = 0;
		void Update(const Timer* pTimer) = 0;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;
	};
}
