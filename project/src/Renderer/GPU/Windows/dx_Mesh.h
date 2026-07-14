#pragma once

#include "../../../pch.h"

#include "dx_VertexBuffer.h"
#include "dx_IndexBuffer.h"

class DXMesh{
	DXVertexBuffer* m_pVertexBuffer;
	DXIndexBuffer* m_pIndexBuffer;
public:
	virtual ~DXMesh() = default;

};