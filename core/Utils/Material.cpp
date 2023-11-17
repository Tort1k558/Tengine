#include "Material.h"

#include<fstream>
#include<nlohmann/json.hpp>

#include"Core/Logger.h"

namespace Tengine
{

	void Material::setSubMaterials(std::unordered_map<SubMaterialType, std::shared_ptr<SubMaterial>> subMaterials)
	{
		m_subMaterials = subMaterials;
		AssetManager::SaveMaterial(this);
	}

	void Material::setSubMaterial(SubMaterialType type, std::shared_ptr<SubMaterial> subMaterial)
	{
		m_subMaterials[type] = subMaterial;
		AssetManager::SaveMaterial(this);
	}

	std::unordered_map<SubMaterialType, std::shared_ptr<SubMaterial>> Material::getSubMaterials()
	{
		return m_subMaterials;
	}

	std::shared_ptr<SubMaterial> Material::getSubMaterial(SubMaterialType type)
	{
		if (hasSubMaterial(type))
		{
			return m_subMaterials[type];
		}
		return nullptr;
	}

	bool Material::hasSubMaterial(SubMaterialType type)
	{
		if (m_subMaterials.find(type) == m_subMaterials.end())
		{
			return false;
		}
		return true;
	}

	SubMaterial::SubMaterial(std::shared_ptr<Texture> texture, Vec3 color) : 
		m_texture(texture), m_color(color)
	{

	}

	SubMaterial::SubMaterial(Vec3 color) :
		m_color(color)
	{

	}

	bool SubMaterial::hasTexture()
	{
		if (m_texture)
		{
			return true;
		}
		return false;
	}
}