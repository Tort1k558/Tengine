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

	void bind() final;
	void unbind() final;

	void addVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) final;
	void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) final;
	unsigned int getCountOfIndices() final;
private:
	GLuint m_id;
	std::shared_ptr<IndexBuffer> m_indexBuffer;
	std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
	unsigned int m_elementCount;
};