#include "Texture.h"

#include"Systems/RendererSystem.h"
#include"OpenGL/TextureOpenGL.h"
#include"Core/Logger.h"

namespace Tengine
{
	TextureType Texture::getType()
	{
		return m_type;
	}

	UVec2 Texture::getSize()
	{
		return m_size;
	}

	std::shared_ptr<Texture> Texture::Create(std::shared_ptr<Image> image, TextureType type, TextureFilter filter)
	{
		return Texture::Create(image->getData(), image->getSize(), type, filter);
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

	unsigned int TextureTypeToSize(TextureType type)
	{
		switch (type)
		{
		case TextureType::R8:
			return 1;
		case TextureType::RG8:
			return 2;
		case TextureType::SRGB8:
		case TextureType::RGB8:
			return 3;
		case TextureType::RGBA8:
		case TextureType::SRGBA8:
			return 4;
		case TextureType::DEPTH32F:
			return 4;
		case TextureType::DEPTH24STENCIL8:
			return 4;
		default:
			return 0;
		}
	}

	size_t MultisampleTexture::getCountSamples()
	{
		return m_samples;
	}

	std::shared_ptr<void> MultisampleTexture::getData()
	{
		return nullptr;
	}

	std::shared_ptr<MultisampleTexture> MultisampleTexture::Create(UVec2 size, TextureType type, size_t samples)
	{
		switch (RendererSystem::GetInstance()->getRendererType())
		{
		case RendererType::None:
			Logger::Critical("ERROR::Renderer not initialized!");
			return nullptr;
		case RendererType::OpenGL:
			return std::make_shared<MultisampleTextureOpenGL>(size, type, samples);
		}
		return nullptr;
	}
}