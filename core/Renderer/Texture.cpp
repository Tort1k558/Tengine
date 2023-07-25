#include "Texture.h"

#include"Systems/RendererSystem.h"
#include"OpenGL/TextureOpenGL.h"
#include"Core/Logger.h"

std::shared_ptr<Texture> Texture::Create(void* data, UVec2 size, TextureType type)
{
	switch (System::GetInstance<RendererSystem>()->getRendererType())
	{
	case RendererType::None:
		Logger::Critical("ERROR::Renderer not initialized!");
		return nullptr;
	case RendererType::OpenGL:
		return std::make_shared<TextureOpenGL>(data, size, type);
	}
	return nullptr;
}
