#include "VertexArray.h"

#include"ECS/Systems/RendererSystem.h"
#include"Renderer/OpenGL/VertexArrayOpenGL.h"
#include"Core/Logger.h"

std::shared_ptr<VertexArray> VertexArray::Create()
{
	switch (System::GetInstance<RendererSystem>()->getRendererType())
	{
	case RendererType::None:
		Logger::Critical("ERROR::Renderer not initialized!");
		return nullptr;
	case RendererType::OpenGL:
		return std::make_shared<VertexArrayOpenGL>();
	}
	return nullptr;
}
