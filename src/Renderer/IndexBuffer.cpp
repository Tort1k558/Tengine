#include "IndexBuffer.h"
#include"Systems/RendererSystem.h"
#include"OpenGL/IndexBufferOpenGL.h"
#include<spdlog/spdlog.h>
std::shared_ptr<IndexBuffer> IndexBuffer::Create(unsigned int* indices, unsigned int count)
{
	switch (RendererSystem::GetType())
	{
	case RendererType::None:
		spdlog::critical("ERROR::Renderer not initialized!");
		return nullptr;
	case RendererType::OpenGL:
		return std::make_shared<IndexBufferOpenGL>(indices, count);
	}
	return nullptr;
}
