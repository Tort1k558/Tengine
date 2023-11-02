#pragma once

#include<unordered_map>

#include"Renderer/Texture.h"
#include"Core/AssetManager.h"

namespace Tengine
{

	enum class MaterialTexture
	{
		Diffuse,
		Specular,
		Height,
		Normal,
		Roughness,
		Occlusion,
		Metalness
	};

	class TENGINE Material : public Resource
	{
	public:
		Material() = default;
		void setTextures(std::unordered_map<MaterialTexture, std::shared_ptr<Texture>> textures);
		void setTextureMaterial(MaterialTexture type, std::shared_ptr<Texture> texture);
		std::unordered_map<MaterialTexture, std::shared_ptr<Texture>> getTextures();
		std::shared_ptr<Texture> getTexture(MaterialTexture type);
		bool hasTexture(MaterialTexture type);
	private:
		void save();
		std::unordered_map<MaterialTexture, std::shared_ptr<Texture>> m_textures;
	};
}