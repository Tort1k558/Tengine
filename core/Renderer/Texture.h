#pragma once

#include<memory>
#include<array>

#include"Core/Math.h"
#include"Core/AssetManager.h"

#include"Utils/Image.h"

namespace Tengine
{
	class Resource;
	class ComponentInfo;

	enum class TextureType
	{
		R8,
		RG8,
		RGB8,
		RGBA8,
		DEPTH32F,
		DEPTH24STENCIL8,
		SRGB8,
		SRGBA8,
		RGBA16F
	};
	unsigned int TextureTypeToSize(TextureType type);

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

		virtual std::shared_ptr<void> getData() = 0;
		virtual unsigned int getId() = 0;
		TextureType getType();
		UVec2 getSize();
		static std::shared_ptr<Texture> Create(std::shared_ptr<Image> image, TextureType type, TextureFilter filter = TextureFilter::None);
		static std::shared_ptr<Texture> Create(void* data, UVec2 size, TextureType type, TextureFilter filter = TextureFilter::None);
	protected:
		UVec2 m_size = { 0,0 };
		TextureType m_type = TextureType::RGB8;
		TextureFilter m_filter = TextureFilter::None;
	};

	class TENGINE MultisampleTexture : public Texture
	{
	public:
		std::shared_ptr<void> getData() final;
		size_t getCountSamples();
		static std::shared_ptr<MultisampleTexture> Create(UVec2 size, TextureType type, size_t samples);
	protected:
		size_t m_samples = 1;
	};
}