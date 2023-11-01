#pragma once

#include<unordered_map>

#include "Utils/Mesh.h"

namespace Tengine
{
	class TENGINE Model : public Component, public Resource
	{
	public:
		Model() = default;
		Model(std::shared_ptr<Mesh> mesh);
		std::shared_ptr<Mesh> getMesh() const;
		void setMesh(std::shared_ptr<Mesh> mesh);
		void setSubmeshMaterial(size_t index, std::shared_ptr<Material> material);

		bool hasSubmeshMaterial(size_t index) const;

		std::shared_ptr<Material> getSubmeshMaterial(size_t index) const;
		std::unordered_map<size_t, std::shared_ptr<Material>> getMaterials() const;

		ComponentInfo getInfo() final;
	private:
		void save();
		std::unordered_map<size_t, std::shared_ptr<Material>> m_materials;
		std::shared_ptr<Mesh> m_mesh;
	};
}
