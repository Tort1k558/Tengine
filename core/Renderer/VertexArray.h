#pragma once

#include<memory>

class VertexBuffer;
class IndexBuffer;

class VertexArray
{
public:
	virtual ~VertexArray() = default;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void addVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) = 0;
	virtual void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) = 0;
	virtual unsigned int getCountOfIndices() = 0;

	static std::shared_ptr<VertexArray> Create();
};