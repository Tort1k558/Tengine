#pragma once

#include<vector>

#include"Core/Math.h"
#include"ECS/Component.h"
#include"Renderer/VertexArray.h"
#include"Renderer/VertexBuffer.h"
#include"Renderer/IndexBuffer.h"
#include"Utils/Material.h"

struct Vertex
{
	Vec3 position;
	Vec3 normal;
	Vec2 texCoords;
};

class SubMesh
{
public:
	SubMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	std::shared_ptr<VertexArray> getVertexArray() { return m_vertexArray; }
	std::shared_ptr<Material> getMaterial();
	void setMaterial(std::shared_ptr<Material> material);
	bool hasMaterial();

private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::shared_ptr<VertexArray> m_vertexArray;
	std::shared_ptr<IndexBuffer> m_indexBuffer;
	std::shared_ptr<Material> m_material;
};

class Mesh : public Component
{
public:
	Mesh() = default;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	Mesh(std::shared_ptr<SubMesh> submesh);
	void addSubmesh(std::shared_ptr<SubMesh> submesh);
	std::vector<std::shared_ptr<SubMesh>> getSubmeshes() { return m_submeshes; }

	DisplayInfo getDisplayInfo() final;
	bool hasDisplayInfo() final;
private:
	std::vector<std::shared_ptr<SubMesh>> m_submeshes;
};