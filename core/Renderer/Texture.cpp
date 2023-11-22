#include "Texture.h"

#include"Systems/RendererSystem.h"
#include"OpenGL/TextureOpenGL.h"
#include"Core/Logger.h"

namespace Tengine
{
	std::shared_ptr<Texture> Texture::Create(std::shared_ptr<Image> image, TextureType type, TextureFilter filter)
	{
		switch (RendererSystem::GetInstance()->getRendererType())
		{
		case RendererType::None:
			Logger::Critical("ERROR::Renderer not initialized!");
			return nullptr;
		case RendererType::OpenGL:
			return std::make_shared<TextureOpenGL>(image, type, filter);
		}
		return nullptr;
	}
	std::shared_ptr<Texture> Texture::Create(void* data, UVec2 size, TextureType type, TextureFilter filter)
	{
		switch (RendererSystem::GetInstance()->getRendererType())
		{
		case RendererType::None:
			Logger::Critical("ERROR::Renderer not initialized!");
			return nullptr;
		case RendererType::OpenGL:
			return std::make_shared<TextureOpenGL>(data, size, type, filter);
		}
		return nullptr;
	}
}