#pragma once

#include<memory>
#include"BufferLayout.h"
enum class BufferUsage
{
	Static = 0,
	Dynamic
};
class VertexBuffer
{
public:

	virtual ~VertexBuffer() = default;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	void setLayout(BufferLayout layout);
	BufferLayout getLayout() { return m_layout; }

	static std::shared_ptr<VertexBuffer> Create(float* vertices, unsigned int size, BufferUsage usage);
protected:
	BufferLayout m_layout;
};