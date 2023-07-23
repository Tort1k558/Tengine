#include "Mesh.h"

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
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	m_submeshes.push_back(SubMesh(vertices, indices));
}

Mesh::Mesh(SubMesh submesh)
{
	m_submeshes.push_back(submesh);
}

void Mesh::addSubmesh(SubMesh submesh)
{
	m_submeshes.push_back(submesh);
}
