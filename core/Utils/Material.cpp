#include "Material.h"

#include<fstream>
#include<nlohmann/json.hpp>

#include"Core/Logger.h"

namespace Tengine
{

	void Material::setTextures(std::unordered_map<MaterialTexture, std::shared_ptr<Texture>> textures)
	{
		m_textures = textures;
		save();
	}

	void Material::setTextureMaterial(MaterialTexture type, std::shared_ptr<Texture> texture)
	{
		m_textures[type] = texture;
		save();
	}

	std::unordered_map<MaterialTexture, std::shared_ptr<Texture>> Material::getTextures()
	{
		return m_textures;
	}

	std::shared_ptr<Texture> Material::getTexture(MaterialTexture type)
	{
		if (hasTexture(type))
		{
			return m_textures[type];
		}
		return nullptr;
	}

	bool Material::hasTexture(MaterialTexture type)
	{
		if (m_textures.find(type) == m_textures.end())
		{
			return false;
		}
		return true;
	}

	void Material::save()
	{
		if (!m_path.empty())
		{
			nlohmann::json data;
			for (const auto& texture : m_textures)
			{
				switch (texture.first)
				{
				case MaterialTexture::Diffuse:
					data["Diffuse"] = texture.second->getPath();
					break;
				case MaterialTexture::Specular:
					data["Specular"] = texture.second->getPath();
					break;
				case MaterialTexture::Height:
					data["Height"] = texture.second->getPath();
					break;
				case MaterialTexture::Normal:
					data["Normal"] = texture.second->getPath();
					break;
				case MaterialTexture::Roughness:
					break;
				case MaterialTexture::Occlusion:
					break;
				case MaterialTexture::Metalness:
					break;
				default:
					break;
				}
			}
			std::ofstream file(m_path.parent_path().string() + "/" + m_path.stem().string() + ".material", std::ios_base::out);
			if (file.is_open()) {
				file << data.dump(4);
				file.close();
			}
			else {
				Logger::Critical("ERROR::Material::Failed to save the material!");
			}
		}
	}
}