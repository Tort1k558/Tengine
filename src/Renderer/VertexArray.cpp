#include "VertexArray.h"

#include"Systems/RendererSystem.h"
#include"Renderer/OpenGL/VertexArrayOpenGL.h"
#include"Core/Logger.h"

std::shared_ptr<VertexArray> VertexArray::Create()
{
	switch (System::GetInstance<RendererSystem>()->getType())
	{
	case RendererType::None:
		Logger::Critical("ERROR::Renderer not initialized!");
		return nullptr;
	case RendererType::OpenGL:
		return std::make_shared<VertexArrayOpenGL>();
	}
	return nullptr;
}
