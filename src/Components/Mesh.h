#pragma once

#include<vector>

#include"Core/Math.h"
#include"ECS/Component.h"
#include"Renderer/VertexArray.h"
#include"Renderer/VertexBuffer.h"
#include"Renderer/IndexBuffer.h"

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
private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::shared_ptr<VertexArray> m_vertexArray;
	std::shared_ptr<IndexBuffer> m_indexBuffer;
};

class Mesh : public Component
{
public:
	Mesh() = default;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	Mesh(SubMesh submesh);
	void addSubmesh(SubMesh submesh);
	std::vector<SubMesh> getSubmeshes() { return m_submeshes; }
private:
	std::vector<SubMesh> m_submeshes;
};