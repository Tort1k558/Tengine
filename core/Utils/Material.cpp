#include "Material.h"

void Material::setTextureMaterial(MaterialTexture type, std::shared_ptr<Texture> texture)
{
	m_textures[type] = texture;
}

std::unordered_map<MaterialTexture, std::shared_ptr<Texture>> Material::getTextures()
{
	return m_textures;
}

std::shared_ptr<Texture> Material::getTexture(MaterialTexture type)
{
	if (m_textures.find(type) == m_textures.end())
	{
		return nullptr;
	}
	return m_textures[type];
}
