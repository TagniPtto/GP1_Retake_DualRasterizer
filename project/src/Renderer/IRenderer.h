#pragma once
#include "../pch.h"
#include "../Mesh.h"
#include "../Timer.h"


class IRenderer {
public:
	IRenderer() = default;
	virtual ~IRenderer() = default;

	virtual void Initialize(SDL_Window* windowHandle) = 0;
	virtual void Render() const = 0;
	virtual void DrawMesh(const Mesh& mesh) const = 0;
	virtual void Update(const dae::Timer* pTimer) = 0;

	IRenderer(const IRenderer&) = delete;
	IRenderer(IRenderer&&) noexcept = delete;
	IRenderer& operator=(const IRenderer&) = delete;
	IRenderer& operator=(IRenderer&&) noexcept = delete;
};