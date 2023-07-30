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
std::shared_ptr<Material> SubMesh::getMaterial()
{
	return m_material;
}

void SubMesh::setMaterial(std::shared_ptr<Material> material)
{
	m_material = material;
}

bool SubMesh::hasMaterial()
{
	if (m_material)
	{
		return true;
	}
	return false;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	m_submeshes.push_back(std::make_shared<SubMesh>(vertices, indices));
}

Mesh::Mesh(std::shared_ptr<SubMesh> submesh)
{
	m_submeshes.push_back(submesh);
}

void Mesh::addSubmesh(std::shared_ptr<SubMesh> submesh)
{
	m_submeshes.push_back(submesh);
}

DisplayInfo Mesh::getDisplayInfo()
{
	DisplayInfo displayInfo;
	displayInfo.setComponentName("Mesh");
	std::shared_ptr<DisplayInfoElementCollapsingHeader> submeshesHeader = std::make_shared<DisplayInfoElementCollapsingHeader>();
	submeshesHeader->name = "Submeshes";
	for (size_t i = 0; i < m_submeshes.size(); i++)
	{
		std::shared_ptr<DisplayInfoElementCollapsingHeader> submeshHeader = std::make_shared<DisplayInfoElementCollapsingHeader>();
		submeshHeader->name = "Submesh" + std::to_string(i);
		std::shared_ptr<Material> material = m_submeshes[i]->getMaterial();
		std::shared_ptr<DisplayInfoElementImage> diffuse = std::make_shared<DisplayInfoElementImage>();
		diffuse->name = "Diffuse Texture";
		std::shared_ptr<DisplayInfoElementImage> specular = std::make_shared<DisplayInfoElementImage>();
		specular->name = "Specular Texture";
		std::shared_ptr<DisplayInfoElementImage> normals = std::make_shared<DisplayInfoElementImage>();
		normals->name = "Normal Texture";
		if (material)
		{
			diffuse->texture = material->getTexture(MaterialTexture::Diffuse);
			specular->texture = material->getTexture(MaterialTexture::Specular);
			normals->texture = material->getTexture(MaterialTexture::Normal);
		}
		submeshHeader->elements.push_back(diffuse);
		submeshHeader->elements.push_back(specular);
		submeshHeader->elements.push_back(normals);
		submeshesHeader->elements.push_back(submeshHeader);
	}
	displayInfo.addElement(submeshesHeader);
	return displayInfo;
}

bool Mesh::hasDisplayInfo()
{
	return true;
}
