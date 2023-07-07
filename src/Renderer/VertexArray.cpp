#include "VertexArray.h"

#include"Systems/RendererSystem.h"
#include"Renderer/OpenGL/VertexArrayOpenGL.h"
std::shared_ptr<VertexArray> VertexArray::Create()
{
	switch (RendererSystem::GetType())
	{
	case RendererType::None:
		spdlog::critical("ERROR::Renderer not initialized!");
		return nullptr;
	case RendererType::OpenGL:
		return std::make_shared<VertexArrayOpenGL>();
	}
	return nullptr;
}
