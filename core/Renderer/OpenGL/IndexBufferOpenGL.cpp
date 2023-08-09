#include "IndexBufferOpenGL.h"

namespace Tengine
{

	IndexBufferOpenGL::IndexBufferOpenGL(GLuint* indices, unsigned int count) :
		m_count(count)
	{
		glCreateBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, m_count * sizeof(GLuint), indices, GL_STATIC_DRAW);
		unbind();
	}

	IndexBufferOpenGL::~IndexBufferOpenGL()
	{
		glDeleteBuffers(1, &m_id);
	}

	IndexBufferOpenGL::IndexBufferOpenGL(IndexBufferOpenGL&& buffer) noexcept :
		m_id(buffer.m_id), m_count(buffer.m_count)
	{
		buffer.m_id = 0;
		buffer.m_count = 0;
	}

	IndexBufferOpenGL& IndexBufferOpenGL::operator=(IndexBufferOpenGL&& buffer) noexcept
	{
		m_id = buffer.m_id;
		m_count = buffer.m_count;

		buffer.m_id = 0;
		buffer.m_count = 0;

		return *this;
	}

	void IndexBufferOpenGL::bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBufferOpenGL::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}