#pragma once

#include<memory>
#include"Renderer/OpenGL/RendererContextOpenGL.h"
#include"Renderer/VertexBuffer.h"

class VertexBufferOpenGL : public VertexBuffer
{
public:
	VertexBufferOpenGL(float* vertices, unsigned int size, BufferUsage usage);
	~VertexBufferOpenGL();

	VertexBufferOpenGL(const VertexBufferOpenGL&) = delete;
	VertexBufferOpenGL& operator=(const VertexBufferOpenGL&) = delete;
	VertexBufferOpenGL(VertexBufferOpenGL&& buffer) noexcept;
	VertexBufferOpenGL& operator=(VertexBufferOpenGL&& buffer) noexcept;

	void bind() override;
	void unbind() override;
private:
	GLuint m_id;
};