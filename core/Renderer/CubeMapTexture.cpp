#include"CubeMapTexture.h"

#include"Systems/RendererSystem.h"
#include"Renderer/OpenGL/CubeMapTextureOpenGL.h"
#include"Core/Logger.h"

namespace Tengine
{
	std::shared_ptr<CubeMapTexture> CubeMapTexture::Create(std::array<std::shared_ptr<Image>, 6> images, std::array<TextureType, 6> types, TextureFilter filter)
	{
		switch (RendererSystem::GetInstance()->getRendererType())
		{
		case RendererType::None:
			Logger::Critical("ERROR::Renderer not initialized!");
			return nullptr;
		case RendererType::OpenGL:
			return std::make_shared<CubeMapTextureOpenGL>(images, types, filter);
		}
		return nullptr;
	}
}