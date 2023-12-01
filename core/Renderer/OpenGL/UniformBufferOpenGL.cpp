#include "UniformBufferOpenGL.h"

namespace Tengine
{
	UniformBufferOpenGL::UniformBufferOpenGL(unsigned int size, const void* data, unsigned int offset)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_UNIFORM_BUFFER, m_id);
		glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	
	void UniformBufferOpenGL::setBinding(unsigned int binding)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_id);
	}

	void UniformBufferOpenGL::setData(unsigned int size, const void* data, unsigned int offset)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_id);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}
