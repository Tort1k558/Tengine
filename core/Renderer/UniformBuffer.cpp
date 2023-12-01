#include "UniformBuffer.h"

#include"Systems/RendererSystem.h"
#include"Core/Logger.h"
#include"Renderer/OpenGL/UniformBufferOpenGL.h"

namespace Tengine
{
    std::shared_ptr<UniformBuffer> UniformBuffer::Create(unsigned int size, const void* data, unsigned int offset)
    {
		switch (RendererSystem::GetInstance()->getRendererType())
		{
		case RendererType::None:
			Logger::Critical("ERROR::Renderer not initialized!");
			return nullptr;
		case RendererType::OpenGL:
			return std::make_shared<UniformBufferOpenGL>(size, data, offset);
		}
		return nullptr;
    }
}