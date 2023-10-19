#include "Mesh.h"

#include"Core/AssetManager.h"
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

	ComponentInfo Mesh::getInfo()
	{
		ComponentInfo displayInfo;

		displayInfo.setComponentName("Mesh");
		std::shared_ptr<FieldFile> loadMeshButton = std::make_shared<FieldFile>();
		loadMeshButton->name = "Load Mesh";
		if (!m_path.empty())
		{
			loadMeshButton->path = m_path;
		}
		loadMeshButton->callback = [this](const std::string& path)
		{
			std::shared_ptr<Object> previousParent = this->getParent();
			if (path == "Primitive::Quad")
			{
				*this = *Primitives::CreateQuad();
				
			}
			else if (path == "Primitive::Cube")
			{
				*this = *Primitives::CreateCube();
			}
			else if (path.find("Primitive::Sphere::") != std::string::npos)
			{
				size_t sectorsPos = path.find("::Sectors::");
				size_t stacksPos = path.rfind("::Stacks::");

				int sectors;
				int stacks;

				if (sectorsPos == std::string::npos || stacksPos == std::string::npos || sectorsPos == stacksPos) {
					sectors = 1;
					stacks = 1;
				}
				sectors = std::stoi(path.substr(sectorsPos + 11, stacksPos - sectorsPos - 11));
				stacks = std::stoi(path.substr(stacksPos + 10));
				*this = *Primitives::CreateSphere(sectors, stacks);
			}
			else
			{
				*this = *AssetManager::LoadMesh(path);
			}
			this->setParent(previousParent);
		};
		displayInfo.addElement(loadMeshButton);
		std::shared_ptr<FiledCollapsingHeader> submeshesHeader = std::make_shared<FiledCollapsingHeader>();
		submeshesHeader->name = "Submeshes";
		for (size_t i = 0; i < m_submeshes.size(); i++)
		{
			std::shared_ptr<FiledCollapsingHeader> submeshHeader = std::make_shared<FiledCollapsingHeader>();
			submeshHeader->name = "Submesh" + std::to_string(i);
			std::shared_ptr<Material> material = m_submeshes[i]->getMaterial();
			std::shared_ptr<FieldImage> diffuse = std::make_shared<FieldImage>();
			diffuse->name = "Diffuse Texture";
			diffuse->size = { 75,75 };
			std::shared_ptr<FieldImage> specular = std::make_shared<FieldImage>();
			specular->name = "Specular Texture";
			specular->size = { 75,75 };
			std::shared_ptr<FieldImage> normals = std::make_shared<FieldImage>();
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
}