#include "VertexBuffer.h"

#include"Renderer.h"
#include"OpenGL/VertexBufferOpenGL.h"
#include<spdlog/spdlog.h>

void VertexBuffer::setLayout(BufferLayout layout)
{
	m_layout = layout;
}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, unsigned int size, BufferUsage usage)
{
	switch (Renderer::GetType())
	{
	case RendererType::None:
		spdlog::critical("ERROR::Renderer not initialized!");
		return nullptr;
	case RendererType::OpenGL:
		return std::make_shared<VertexBufferOpenGL>(vertices, size, usage);
	}
	return nullptr;
}
