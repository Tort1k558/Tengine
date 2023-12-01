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
		size_t getMaterialIndex() const;
		void setMaterialIndex(size_t index);
	private:
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		std::shared_ptr<VertexArray> m_vertexArray;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
		size_t m_materialIndex;
	};

	class TENGINE Mesh : public Resource
	{
	public:
		Mesh() = default;
		void addSubmesh(std::shared_ptr<SubMesh> submesh);
		bool hasSubmeshes() const;
		std::vector<std::shared_ptr<SubMesh>> getSubmeshes() const;
	private:
		std::vector<std::shared_ptr<SubMesh>> m_submeshes;
	};
}