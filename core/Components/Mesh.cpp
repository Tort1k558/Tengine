#include "Mesh.h"

#include"Core/AssetManager.h"
#include"Scene/SceneSerializer.h"
#include"Utils/Primitives.h"

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
	std::shared_ptr<Material> SubMesh::getMaterial() const
	{
		return m_material;
	}

	void SubMesh::setMaterial(std::shared_ptr<Material> material)
	{
		m_material = material;
	}

	bool SubMesh::hasMaterial() const
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

	bool Mesh::hasSubmeshes() const
	{
		return !m_submeshes.empty();
	}

	std::vector<std::shared_ptr<SubMesh>> Mesh::getSubmeshes() const
	{
		return m_submeshes;
	}

	DisplayInfo Mesh::getDisplayInfo()
	{
		DisplayInfo displayInfo;
		displayInfo.setComponentName("Mesh");
		std::shared_ptr<DisplayInfoElementFileDialog> loadMeshButton = std::make_shared<DisplayInfoElementFileDialog>();
		loadMeshButton->name = "Load Mesh";
		loadMeshButton->callback = [this](const std::string& path)
		{
			std::shared_ptr<Mesh> newMesh = AssetManager::LoadMesh(path);
			if (newMesh)
			{
				newMesh->setParent(this->getParent());
			}
			this->getParent()->addComponent(newMesh);
		};
		displayInfo.addElement(loadMeshButton);
		std::shared_ptr<DisplayInfoElementCollapsingHeader> submeshesHeader = std::make_shared<DisplayInfoElementCollapsingHeader>();
		submeshesHeader->name = "Submeshes";
		for (size_t i = 0; i < m_submeshes.size(); i++)
		{
			std::shared_ptr<DisplayInfoElementCollapsingHeader> submeshHeader = std::make_shared<DisplayInfoElementCollapsingHeader>();
			submeshHeader->name = "Submesh" + std::to_string(i);
			std::shared_ptr<Material> material = m_submeshes[i]->getMaterial();
			std::shared_ptr<DisplayInfoElementImage> diffuse = std::make_shared<DisplayInfoElementImage>();
			diffuse->name = "Diffuse Texture";
			diffuse->size = { 75,75 };
			std::shared_ptr<DisplayInfoElementImage> specular = std::make_shared<DisplayInfoElementImage>();
			specular->name = "Specular Texture";
			specular->size = { 75,75 };
			std::shared_ptr<DisplayInfoElementImage> normals = std::make_shared<DisplayInfoElementImage>();
			normals->name = "Normal Texture";
			normals->size = { 75,75 };
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

	void Mesh::serialize(nlohmann::json& data)
	{
		//Serialize
		if (!getPath().empty())
		{
			data["mesh"]["path"] = getPath().string();
		}
	}
}