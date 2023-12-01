#include "Material.h"

#include<fstream>
#include<nlohmann/json.hpp>

#include"Core/Logger.h"

namespace Tengine
{
	SubMaterial::SubMaterial(std::shared_ptr<Texture> texture, Vec3 color) :
		m_texture(texture), m_color(color)
	{

	}

	SubMaterial::SubMaterial(Vec3 color) :
		m_color(color)
	{

	}

	void SubMaterial::setTexture(std::shared_ptr<Texture> texture)
	{
		m_texture = texture;
		saveParent();
	}

	std::shared_ptr<Texture> SubMaterial::getTexture() const
	{
		return m_texture;
	}

	bool SubMaterial::hasTexture()
	{
		if (m_texture)
		{
			return true;
		}
		return false;
	}
	void SubMaterial::setColor(Vec3 color)
	{
		m_color = color;
		saveParent();
	}

	Vec3 SubMaterial::getColor() const
	{
		return m_color;
	}

	void SubMaterial::saveParent()
	{
		if (m_parent)
		{
			AssetManager::SaveMaterial(m_parent);
		}
	}

	void Material::setSubMaterials(std::unordered_map<SubMaterialType, std::shared_ptr<SubMaterial>> subMaterials)
	{
		for (auto& subMaterial : m_subMaterials)
		{
			subMaterial.second->m_parent = nullptr;
		}
		m_subMaterials = subMaterials;
		for (auto& subMaterial : m_subMaterials)
		{
			subMaterial.second->m_parent = this;
		}
		AssetManager::SaveMaterial(this);
	}

	void Material::setSubMaterial(SubMaterialType type, std::shared_ptr<SubMaterial> subMaterial)
	{
		if (hasSubMaterial(type))
		{
			m_subMaterials[type]->m_parent = nullptr;
		}
		subMaterial->m_parent = this;
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

	void Material::removeSubMaterial(SubMaterialType type)
	{
		if (hasSubMaterial(type))
		{
			m_subMaterials.erase(type);
		}
		AssetManager::SaveMaterial(this);
	}

}