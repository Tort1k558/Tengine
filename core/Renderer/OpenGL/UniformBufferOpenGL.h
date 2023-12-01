#pragma once
#include"Renderer/UniformBuffer.h"
#include"Renderer/OpenGL/RendererContextOpenGL.h"

namespace Tengine
{

	class UniformBufferOpenGL : public UniformBuffer
	{
	public:
		UniformBufferOpenGL(unsigned int size, const void* data = nullptr, unsigned int offset = 0);
		void setBinding(unsigned int binding) final;
		void setData(unsigned int size, const void* data, unsigned int offset = 0) final;

	private:
		GLuint m_id = 0;
	};
}
