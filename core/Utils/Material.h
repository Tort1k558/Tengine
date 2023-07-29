#pragma once

#include<unordered_map>

#include"Renderer/Texture.h"

enum class MaterialTexture
{
	Diffuse,
	Height,
	Normal
};

class Material
{
public:
	Material() = default;
	void setTextures(std::unordered_map<MaterialTexture, std::shared_ptr<Texture>> textures);
	void setTextureMaterial(MaterialTexture type, std::shared_ptr<Texture> texture);
	std::unordered_map<MaterialTexture, std::shared_ptr<Texture>> getTextures();
	std::shared_ptr<Texture> getTexture(MaterialTexture type);
	bool hasTexture(MaterialTexture type);
private:
	std::unordered_map<MaterialTexture, std::shared_ptr<Texture>> m_textures;
};