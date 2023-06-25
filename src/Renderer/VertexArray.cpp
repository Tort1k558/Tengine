#include "VertexArray.h"

#include"Renderer.h"
#include"Renderer/OpenGL/VertexArrayOpenGL.h"
std::shared_ptr<VertexArray> VertexArray::Create()
{
	switch (Renderer::GetType())
	{
	case RendererType::None:
		spdlog::critical("ERROR::Renderer not initialized!");
		return nullptr;
	case RendererType::OpenGL:
		return std::make_shared<VertexArrayOpenGL>();
	}
	return nullptr;
}
