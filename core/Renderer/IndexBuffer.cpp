#include "IndexBuffer.h"

#include"ECS/Systems/RendererSystem.h"
#include"OpenGL/IndexBufferOpenGL.h"
#include"Core/Logger.h"

namespace Tengine
{

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(unsigned int* indices, unsigned int count)
	{
		switch (RendererSystem::GetInstance()->getRendererType())
		{
		case RendererType::None:
			Logger::Critical("ERROR::Renderer not initialized!");
			return nullptr;
		case RendererType::OpenGL:
			return std::make_shared<IndexBufferOpenGL>(indices, count);
		}
		return nullptr;
	}
}