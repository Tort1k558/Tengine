#pragma once

#include<memory>
#include"Renderer/OpenGL/RendererContextOpenGL.h"
#include"Renderer/IndexBuffer.h"

class IndexBufferOpenGL : public IndexBuffer
{
public:
	IndexBufferOpenGL(GLuint* indices, unsigned int count);
	~IndexBufferOpenGL();
	
	IndexBufferOpenGL(const IndexBufferOpenGL&) = delete;
	IndexBufferOpenGL& operator=(const IndexBufferOpenGL&) = delete;
	IndexBufferOpenGL(IndexBufferOpenGL&& buffer) noexcept;
	IndexBufferOpenGL& operator=(IndexBufferOpenGL&& buffer) noexcept;

	void bind() override;
	void unbind() override;
private:
	GLuint m_id;
	GLuint m_count;
};