#pragma once

#include<memory>
#include"Renderer/OpenGL/RendererContextOpenGL.h"
#include"Renderer/VertexBuffer.h"

namespace Tengine
{
	class VertexBufferOpenGL : public VertexBuffer
	{
	public:
		VertexBufferOpenGL(float* data, unsigned int size, BufferUsage usage);
		~VertexBufferOpenGL();

		VertexBufferOpenGL(const VertexBufferOpenGL&) = delete;
		VertexBufferOpenGL& operator=(const VertexBufferOpenGL&) = delete;
		VertexBufferOpenGL(VertexBufferOpenGL&& buffer) noexcept;
		VertexBufferOpenGL& operator=(VertexBufferOpenGL&& buffer) noexcept;

		void bind() final;
		void unbind() final;
	private:
		GLuint m_id;
	};
}