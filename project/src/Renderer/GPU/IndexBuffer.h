#pragma once


class IndexBuffer {
public:
	virtual ~IndexBuffer() = default;

	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
};
