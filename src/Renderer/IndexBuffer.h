#pragma once

#include<memory>

class IndexBuffer
{
public:
	virtual ~IndexBuffer() = default;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	static std::shared_ptr<IndexBuffer> Create(unsigned int* indices, unsigned int count);
};