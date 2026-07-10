#pragma once


class VertexBuffer {
public:
	virtual ~VertexBuffer() = default;

	virtual void SetData();

	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
};

