#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertexes, std::vector<unsigned int> indices) :
	m_vertexes(vertexes), m_indices(indices)
{
	for (const auto& vertex : m_vertexes)
	{
		m_data.push_back(vertex.position.x);
		m_data.push_back(vertex.position.y);
		m_data.push_back(vertex.position.z);
		m_data.push_back(vertex.normal.x);
		m_data.push_back(vertex.normal.y);
		m_data.push_back(vertex.normal.z);
		m_data.push_back(vertex.texCoords.x);
		m_data.push_back(vertex.texCoords.y);
	}

	std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(&m_data[0], sizeof(float) * m_data.size(), BufferUsage::Static);
	BufferLayout layout({ {ElementType::Float3},{ElementType::Float3},{ElementType::Float2} });
	vb->setLayout(layout);
	m_vertexArray = VertexArray::Create();
	m_vertexArray->addVertexBuffer(vb);
	if (m_indices.size() != 0)
	{
		m_indexBuffer = IndexBuffer::Create(&m_indices[0], m_indices.size());
		m_vertexArray->setIndexBuffer(m_indexBuffer);
	}
}
