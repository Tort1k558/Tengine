#include "VertexBufferOpenGL.h"

int bufferUsageToOpenGL(BufferUsage usage)
{
	switch (usage)
	{
	case BufferUsage::Static:
		return GL_STATIC_DRAW;
	case BufferUsage::Dynamic:
		return GL_DYNAMIC_DRAW;
	default:
		spdlog::critical("ERROR::SHADER::Unknown type of Vertex Buffer: {0}", static_cast<int>(usage));
		break;
	}
	return 0;
}

VertexBufferOpenGL::VertexBufferOpenGL(float* vertices, unsigned int size, BufferUsage usage)
{
	glCreateBuffers(1, &m_id);
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, vertices, bufferUsageToOpenGL(usage));
	unbind();
}

VertexBufferOpenGL::~VertexBufferOpenGL()
{
	glDeleteBuffers(1, &m_id);
}

VertexBufferOpenGL::VertexBufferOpenGL(VertexBufferOpenGL&& buffer) noexcept :
	m_id(buffer.m_id)
{
	buffer.m_id = 0;
}

VertexBufferOpenGL& VertexBufferOpenGL::operator=(VertexBufferOpenGL&& buffer) noexcept
{
	m_id = buffer.m_id;
	buffer.m_id = 0;
	return *this;
}

void VertexBufferOpenGL::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBufferOpenGL::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
