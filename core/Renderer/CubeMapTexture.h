#pragma once

#include<array>

#include"Core/AssetManager.h"
#include"Renderer/Texture.h"

namespace Tengine
{
	enum class CubeMapSide
	{
		Right = 0,
		Left,
		Top,
		Bottom,
		Front,
		Back
	};
	class TENGINE CubeMapTexture : public Resource
	{
	public:
		virtual ~CubeMapTexture() = default;

		std::array<std::shared_ptr<Texture>, 6> getTextures();
		std::shared_ptr<Texture> getTexture(CubeMapSide side);
		
		virtual void bind(unsigned int slot) = 0;
		virtual unsigned int getId() = 0;
		virtual void setSideTexture(CubeMapSide side, std::shared_ptr<Texture> texture) = 0;
		virtual std::shared_ptr<void> getData(CubeMapSide side) = 0;
		
		static std::shared_ptr<CubeMapTexture> Create(std::array<std::shared_ptr<Texture>, 6> textures = {}, TextureFilter filter = TextureFilter::None);
	protected:
		std::array<std::shared_ptr<Texture>, 6> m_textures;
		TextureFilter m_filter = TextureFilter::None;
	};
}