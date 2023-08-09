#include "VertexBuffer.h"

#include"ECS/Systems/RendererSystem.h"
#include"OpenGL/VertexBufferOpenGL.h"
#include"Core/Logger.h"

namespace Tengine
{
	void VertexBuffer::setLayout(BufferLayout layout)
	{
		m_layout = layout;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, unsigned int size, BufferUsage usage)
	{
		switch (System::GetInstance<RendererSystem>()->getRendererType())
		{
		case RendererType::None:
			Logger::Critical("ERROR::Renderer not initialized!");
			return nullptr;
		case RendererType::OpenGL:
			return std::make_shared<VertexBufferOpenGL>(vertices, size, usage);
		}
		return nullptr;
	}
}