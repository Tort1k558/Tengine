#pragma once

#include<unordered_map>

#include"Renderer/Texture.h"
#include"Core/AssetManager.h"

namespace Tengine
{

	enum class SubMaterialType
	{
		Diffuse,
		Normal,
		Specular,
		Height,
		Roughness,
		Occlusion,
		Metalness
	};
	
	class TENGINE SubMaterial
	{
	public:
		SubMaterial() = default;
		SubMaterial(std::shared_ptr<Texture> texture, Vec3 color = { 0.0f, 0.0f, 0.0f });
		SubMaterial(Vec3 color);

		void setTexture(std::shared_ptr<Texture> texture);
		std::shared_ptr<Texture> getTexture() const;
		bool hasTexture();

		void setColor(Vec3 color);
		Vec3 getColor() const;

	private:
		void saveParent();

		std::shared_ptr<Texture> m_texture = nullptr;
		Vec3 m_color = { 0.0f, 0.0f, 0.0f };

		Material* m_parent = nullptr;
		friend class Material;
	};

	class TENGINE Material : public Resource
	{
	public:
		Material() = default;
		void setSubMaterials(std::unordered_map<SubMaterialType, std::shared_ptr<SubMaterial>> subMaterials);
		std::unordered_map<SubMaterialType, std::shared_ptr<SubMaterial>> getSubMaterials();
		void setSubMaterial(SubMaterialType type, std::shared_ptr<SubMaterial> subMaterial);
		std::shared_ptr<SubMaterial> getSubMaterial(SubMaterialType type);
		bool hasSubMaterial(SubMaterialType type);

		void removeSubMaterial(SubMaterialType type);

	private:
		std::unordered_map<SubMaterialType, std::shared_ptr<SubMaterial>> m_subMaterials;
	};
}