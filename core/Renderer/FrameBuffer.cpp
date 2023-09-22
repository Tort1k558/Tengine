#include "FrameBuffer.h"

#include"Systems/RendererSystem.h"
#include"Core/Logger.h"
#include"OpenGL/FrameBufferOpenGL.h"
namespace Tengine
{
	std::shared_ptr<FrameBuffer> Tengine::FrameBuffer::Create(UVec2 size)
	{
		switch (RendererSystem::GetInstance()->getRendererType())
		{
		case RendererType::None:
			Logger::Critical("ERROR::Renderer not initialized!");
			return nullptr;
		case RendererType::OpenGL:
			return std::make_shared<FrameBufferOpenGL>(size);
		}
		return nullptr;
	}
}
