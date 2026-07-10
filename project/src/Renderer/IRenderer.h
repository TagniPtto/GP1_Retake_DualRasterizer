#pragma once
#include "../pch.h"


namespace dae {
	class IRenderer {
	public:
		IRenderer() = default;
		virtual ~IRenderer() = default;

		virtual void Initialize(SDL_Window* windowHandle) = 0;
		virtual void Render() const = 0;
		virtual void Update(const Timer* pTimer) = 0;

		IRenderer(const IRenderer&) = delete;
		IRenderer(IRenderer&&) noexcept = delete;
		IRenderer& operator=(const IRenderer&) = delete;
		IRenderer& operator=(IRenderer&&) noexcept = delete;
	};
}
