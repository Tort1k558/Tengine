#include "Mesh.h"

#include<fstream>

#include"Core/AssetManager.h"
#include"Core/Logger.h"
namespace Tengine
{

	SubMesh::SubMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) :
		m_vertices(vertices), m_indices(indices)
	{
		if (m_vertices.size() == 0)
		{
			return;
		}
		std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(&(m_vertices[0].position.x), static_cast<unsigned int>(sizeof(Vertex) * m_vertices.size()), BufferUsage::Static);
		BufferLayout layout({ {ElementType::Float3},{ElementType::Float3},{ElementType::Float2} });
		vb->setLayout(layout);
		m_vertexArray = VertexArray::Create();
		m_vertexArray->addVertexBuffer(vb);
		if (m_indices.size() != 0)
		{
			m_indexBuffer = IndexBuffer::Create(&m_indices[0], static_cast<unsigned int>(m_indices.size()));
			m_vertexArray->setIndexBuffer(m_indexBuffer);
		}
	}
	std::shared_ptr<VertexArray> SubMesh::getVertexArray() const
	{
		return m_vertexArray;
	}

	size_t SubMesh::getMaterialIndex()
	{
		return m_materialIndex;
	}

	void SubMesh::setMaterialIndex(size_t index)
	{
		m_materialIndex = index;
	}

	void Mesh::addSubmesh(std::shared_ptr<SubMesh> submesh)
	{
		m_submeshes.push_back(submesh);
	}

	bool Mesh::hasSubmeshes() const
	{
		return !m_submeshes.empty();
	}

	std::vector<std::shared_ptr<SubMesh>> Mesh::getSubmeshes() const
	{
		return m_submeshes;
	}
}