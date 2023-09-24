#pragma once

#include<vector>

#include"Core/Math.h"
#include"Core/AssetManager.h"
#include"ECS/Component.h"
#include"Renderer/VertexArray.h"
#include"Renderer/VertexBuffer.h"
#include"Renderer/IndexBuffer.h"
#include"Utils/Material.h"

namespace Tengine
{

	struct TENGINE Vertex
	{
		Vec3 position;
		Vec3 normal;
		Vec2 texCoords;
	};

	class TENGINE SubMesh
	{
	public:
		SubMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		std::shared_ptr<VertexArray> getVertexArray() const;
		std::shared_ptr<Material> getMaterial() const;
		void setMaterial(std::shared_ptr<Material> material);
		bool hasMaterial() const;

	private:
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		std::shared_ptr<VertexArray> m_vertexArray;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
		std::shared_ptr<Material> m_material;
	};

	class TENGINE Mesh : public Component, public Resource
	{
	public:
		Mesh() = default;
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		Mesh(std::shared_ptr<SubMesh> submesh);
		void addSubmesh(std::shared_ptr<SubMesh> submesh);
		bool hasSubmeshes() const;
		std::vector<std::shared_ptr<SubMesh>> getSubmeshes() const;

		void serialize(nlohmann::json& data) final;
		DisplayInfo getDisplayInfo() override;
	private:
		std::vector<std::shared_ptr<SubMesh>> m_submeshes;
	};
}