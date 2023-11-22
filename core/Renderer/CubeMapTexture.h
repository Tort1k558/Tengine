#pragma once

#include<array>

#include"Core/AssetManager.h"
#include"Renderer/Texture.h"

namespace Tengine
{
	class TENGINE CubeMapTexture : public Resource
	{
	public:
		virtual ~CubeMapTexture() = default;

		virtual void bind(unsigned int slot) = 0;

		virtual unsigned int getId() = 0;

		static std::shared_ptr<CubeMapTexture> Create(std::array<std::shared_ptr<Image>, 6> images, std::array<TextureType, 6> types, TextureFilter filter = TextureFilter::None);
	protected:
		TextureFilter m_filter = TextureFilter::None;
	};
}