#include "VertexBuffer.h"

#include"Systems/RendererSystem.h"
#include"OpenGL/VertexBufferOpenGL.h"
#include"Core/Logger.h"

namespace Tengine
{
	void VertexBuffer::setLayout(BufferLayout layout)
	{
		m_layout = layout;
	}

	BufferLayout VertexBuffer::getLayout()
	{
		return m_layout;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* data, unsigned int size, BufferUsage usage)
	{
		switch (RendererSystem::GetInstance()->getRendererType())
		{
		case RendererType::None:
			Logger::Critical("ERROR::Renderer not initialized!");
			return nullptr;
		case RendererType::OpenGL:
			return std::make_shared<VertexBufferOpenGL>(data, size, usage);
		}
		return nullptr;
	}
}