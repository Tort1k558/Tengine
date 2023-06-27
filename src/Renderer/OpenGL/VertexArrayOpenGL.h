#pragma once

#include"Renderer/VertexArray.h"
#include"Renderer/OpenGL/RendererContextOpenGL.h"
#include"Renderer/IndexBuffer.h"
#include "Renderer/VertexBuffer.h"

class VertexArrayOpenGL : public VertexArray
{
public:
	VertexArrayOpenGL();
	~VertexArrayOpenGL();

	VertexArrayOpenGL(const VertexArrayOpenGL&) = delete;
	VertexArrayOpenGL& operator=(const VertexArrayOpenGL&) = delete;
	VertexArrayOpenGL(VertexArrayOpenGL&& array) noexcept;
	VertexArrayOpenGL& operator=(VertexArrayOpenGL&& array) noexcept;

	void bind() override;
	void unbind() override;

	void addVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) override;
	void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) override;
	unsigned int getCountOfIndices() override { return m_indexBuffer->getCountOfIndices(); }
private:
	GLuint m_id;
	std::shared_ptr<IndexBuffer> m_indexBuffer;
	unsigned int m_elementCount;
};