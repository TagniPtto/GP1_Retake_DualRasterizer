#pragma once

#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "ColorRGB.h"
struct VS_INPUT {
	float position[3];
	float color[3];
	float uv[2];
	float normal[3];
	float tangent[3];
};
struct Vertex_Out
{
	dae::Vector4 position{};
	dae::ColorRGB color{ 1.0f,1.0f,1.0f };
	dae::Vector2 uv{};
	dae::Vector3 normal{};
	dae::Vector3 tangent{};
	dae::Vector3 viewDirection{};
};
enum class PrimitiveTopology
{
	TriangleList,
	TriangleStrip
};
