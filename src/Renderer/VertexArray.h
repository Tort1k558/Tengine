#pragma once

class VertexArray
{
public:
	virtual ~VertexArray() = default;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void addVertexBuffer() = 0;
	virtual void addIndexBuffer() = 0;
};