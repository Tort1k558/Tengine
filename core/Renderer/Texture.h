#pragma once

#include<memory>
#include<filesystem>

#include"Core/Math.h"
#include"Core/AssetManager.h"

namespace Tengine
{
	class Resource;

	enum class TextureType
	{
		R8,
		RG8,
		RGB8,
		RGBA8,
		DEPTH32
	};

	enum class TextureFilter
	{
		None,
		Bilinear,
		Trilinear,
		Anisotropic4,
		Anisotropic8,
		Anisotropic16
	};

	class TENGINE Texture : public Resource
	{
	public:
		virtual ~Texture() = default;

		virtual void bind(unsigned int slot) = 0;

		UVec2 getSize();
		virtual void* getData() = 0;
		virtual unsigned int getId() = 0;

		static std::shared_ptr<Texture> Create(void* data, UVec2 size, TextureType type,TextureFilter filter = TextureFilter::None);

	protected:
		UVec2 m_size;
		void* m_data;
		TextureType m_type;
		TextureFilter m_filter;
	};
}