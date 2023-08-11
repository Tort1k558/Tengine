#include "VertexArray.h"

#include"ECS/Systems/RendererSystem.h"
#include"Renderer/OpenGL/VertexArrayOpenGL.h"
#include"Core/Logger.h"

namespace Tengine
{

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (RendererSystem::GetInstance()->getRendererType())
		{
		case RendererType::None:
			Logger::Critical("ERROR::Renderer not initialized!");
			return nullptr;
		case RendererType::OpenGL:
			return std::make_shared<VertexArrayOpenGL>();
		}
		return nullptr;
	}
}