#include "Mesh.h"

#include<fstream>

#include"Core/AssetManager.h"
#include"Core/Logger.h"
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

	void Mesh::addSubmesh(std::shared_ptr<SubMesh> submesh)
	{
		m_submeshes.push_back(submesh);
		save();
	}

	bool Mesh::hasSubmeshes() const
	{
		return !m_submeshes.empty();
	}

	std::vector<std::shared_ptr<SubMesh>> Mesh::getSubmeshes() const
	{
		return m_submeshes;
	}

	std::filesystem::path Mesh::getPathToMesh()
	{
		return m_pathToMesh;
	}

	void Mesh::setPathToMesh(std::filesystem::path path)
	{
		m_path = path;
	}

	ComponentInfo Mesh::getInfo()
	{
		ComponentInfo displayInfo;

		displayInfo.setComponentName("Mesh");
		std::shared_ptr<FieldFile> loadMeshFile = std::make_shared<FieldFile>();
		loadMeshFile->name = "Load Mesh";
		if (!m_path.empty())
		{
			loadMeshFile->path = m_path;
		}
		loadMeshFile->callback = [this](const std::string& path)
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
				*this = *AssetManager::LoadMesh(path, false);
			}
			this->setParent(previousParent);
		};
		displayInfo.addElement(loadMeshFile);
		std::shared_ptr<FieldCollapsingHeader> submeshesHeader = std::make_shared<FieldCollapsingHeader>();
		submeshesHeader->name = "Submeshes";
		for (size_t i = 0; i < m_submeshes.size(); i++)
		{
			std::shared_ptr<FieldCollapsingHeader> submeshHeader = std::make_shared<FieldCollapsingHeader>();
			submeshHeader->name = "Submesh" + std::to_string(i);
			std::shared_ptr<Material> material = m_submeshes[i]->getMaterial();
			if (material)
			{
				std::shared_ptr<FieldFile> materialFile = std::make_shared<FieldFile>();
				materialFile->name = "Material";
				materialFile->path = material->getPath();
				materialFile->callback = [this,i](const std::string& path)
				{
					this->m_submeshes[i]->setMaterial(AssetManager::LoadMaterial(path));
				};
				//std::shared_ptr<FieldImage> albedo = std::make_shared<FieldImage>();
				//albedo->name = "Albedo Texture";
				//albedo->size = { 75,75 };
				//std::shared_ptr<FieldImage> specular = std::make_shared<FieldImage>();
				//specular->name = "Specular Texture";
				//specular->size = { 75,75 };
				//std::shared_ptr<FieldImage> normals = std::make_shared<FieldImage>();
				//normals->name = "Normal Texture";
				//normals->size = { 75,75 };
				//std::shared_ptr<FieldImage> height = std::make_shared<FieldImage>();
				//height->name = "Height Texture";
				//height->size = { 75,75 };
				//std::shared_ptr<FieldImage> roughness = std::make_shared<FieldImage>();
				//roughness->name = "Roughness Texture";
				//roughness->size = { 75,75 };
				//std::shared_ptr<FieldImage> metalness = std::make_shared<FieldImage>();
				//metalness->name = "Metalness Texture";
				//metalness->size = { 75,75 };
				//if (material)
				//{
				//	albedo->texture = material->getTexture(MaterialTexture::Diffuse);
				//	specular->texture = material->getTexture(MaterialTexture::Specular);
				//	normals->texture = material->getTexture(MaterialTexture::Normal);
				//	height->texture = material->getTexture(MaterialTexture::Height);
				//	roughness->texture = material->getTexture(MaterialTexture::Roughness);
				//	metalness->texture = material->getTexture(MaterialTexture::Metalness);
				//}
				//submeshHeader->elements.push_back(albedo);
				//submeshHeader->elements.push_back(specular);
				//submeshHeader->elements.push_back(normals);
				//submeshHeader->elements.push_back(height);
				//submeshHeader->elements.push_back(roughness);
				//submeshHeader->elements.push_back(metalness);
				submeshHeader->elements.push_back(materialFile);
			}
			submeshesHeader->elements.push_back(submeshHeader);
		}
		displayInfo.addElement(submeshesHeader);

		return displayInfo;
	}
	void Mesh::save()
	{
		nlohmann::json data;
		data["PathToMesh"] = getPath();
		std::vector<std::shared_ptr<SubMesh>> submeshes = getSubmeshes();
		for (size_t i = 0; i < submeshes.size(); i++)
		{
			if (submeshes[i]->hasMaterial())
			{
				data["PathToSubmeshMaterial" + std::to_string(i)] = submeshes[i]->getMaterial()->getPath();
			}
		}
		std::ofstream file(getPath().parent_path().string() + "/" + getPath().stem().string() + ".model", std::ios_base::out);
		if (file.is_open())
		{
			file << data.dump(4);
			file.close();
		}
		else
		{
			Logger::Critical("ERROR::AssetManager::Failed to save the mesh file!");
		}
	}
}