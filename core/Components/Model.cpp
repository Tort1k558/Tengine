#include "Model.h"

#include<fstream>

#include"Core/Logger.h"
#include"Utils/Primitives.h"
#include"Utils/FileManager.h"

#include<assimp/scene.h>
#include<assimp/Importer.hpp>
#include<assimp/postprocess.h>
namespace Tengine
{
	std::shared_ptr<Mesh> Model::getMesh() const
	{
		return m_mesh;
	}

	void Model::setMesh(std::shared_ptr<Mesh> mesh)
	{
		m_mesh = mesh;
		AssetManager::SaveModel(this);
	}

	void Model::setSubmeshMaterial(size_t index, std::shared_ptr<Material> material)
	{
		m_materials[index] = material;
		AssetManager::SaveModel(this);
	}

	bool Model::hasSubmeshMaterial(size_t index) const
	{
		return m_materials.find(index) != m_materials.end();
	}

	std::shared_ptr<Material> Model::getSubmeshMaterial(size_t index) const
	{
		if (hasSubmeshMaterial(index))
		{
			return m_materials.at(index);
		}
		return nullptr;
	}

	std::unordered_map<size_t, std::shared_ptr<Material>> Model::getMaterials() const
	{
		return m_materials;
	}

	ComponentInfo Model::getInfo()
	{
		ComponentInfo componentInfo;

		componentInfo.setComponentName("Model");
		std::shared_ptr<FieldFile> modelFile = std::make_shared<FieldFile>();
		modelFile->name = "PathToModelFile";
		if (!m_path.empty())
		{
			modelFile->path = m_path;
		}
		modelFile->callback = [this](const std::string& path)
		{
			std::shared_ptr<Model> newModel = AssetManager::LoadModel(path);
			newModel->setParent(this->getParent());
			*this = *newModel;

		};
		componentInfo.addElement(modelFile);

		return componentInfo;
	}
    void Model::loadMeshMaterials()
    {
        if (m_mesh && std::filesystem::exists(m_mesh->getPath()))
        {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(m_mesh->getPath().string().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                Logger::Critical("ERROR::ASSIMP::{0}", importer.GetErrorString());
                return;
            }
            auto loadMaterialTexture = [](aiMaterial* material, aiTextureType type, std::filesystem::path directory) -> std::shared_ptr<Texture>
                {
                    if (material->GetTextureCount(type) > 0)
                    {
                        aiString pathToTexture;
                        material->GetTexture(type, 0, &pathToTexture);
                        return AssetManager::LoadTexture(directory.string() + "/" + pathToTexture.C_Str());
                    }
                    return nullptr;
                };
            std::filesystem::path parentPath = m_mesh->getPath().parent_path();
            std::vector<std::shared_ptr<Material>> materials;
            for (size_t i = 0; i < scene->mNumMaterials; i++)
            {
                std::shared_ptr<Material> material = std::make_shared<Material>();
                material->setPath(FileManager::GetUniqueFilePath(parentPath.string() + "/Material.material"));
                aiMaterial* aimaterial = scene->mMaterials[i];
                std::shared_ptr<Texture> diffuse = loadMaterialTexture(aimaterial, aiTextureType_DIFFUSE, parentPath);
                if (diffuse)
                {
                    std::shared_ptr subMaterial = std::make_shared<SubMaterial>(diffuse);
                    material->setSubMaterial(SubMaterialType::Diffuse, subMaterial);
                }
                std::shared_ptr<Texture> normals = loadMaterialTexture(aimaterial, aiTextureType_NORMALS, parentPath);
                if (normals)
                {
                    std::shared_ptr subMaterial = std::make_shared<SubMaterial>(normals);
                    material->setSubMaterial(SubMaterialType::Normal, subMaterial);
                }
                std::shared_ptr<Texture> specular = loadMaterialTexture(aimaterial, aiTextureType_SPECULAR, parentPath);
                if (specular)
                {
                    std::shared_ptr subMaterial = std::make_shared<SubMaterial>(specular);
                    material->setSubMaterial(SubMaterialType::Specular, subMaterial);
                }
                std::shared_ptr<Texture> roughness = loadMaterialTexture(aimaterial, aiTextureType_DIFFUSE_ROUGHNESS, parentPath);
                if (roughness)
                {
                    std::shared_ptr subMaterial = std::make_shared<SubMaterial>(roughness);
                    material->setSubMaterial(SubMaterialType::Roughness, subMaterial);
                }
                std::shared_ptr<Texture> occlusion = loadMaterialTexture(aimaterial, aiTextureType_AMBIENT_OCCLUSION, parentPath);
                if (occlusion)
                {
                    std::shared_ptr subMaterial = std::make_shared<SubMaterial>(occlusion);
                    material->setSubMaterial(SubMaterialType::Occlusion, subMaterial);
                }
                std::shared_ptr<Texture> metalness = loadMaterialTexture(aimaterial, aiTextureType_METALNESS, parentPath);
                if (metalness)
                {
                    std::shared_ptr subMaterial = std::make_shared<SubMaterial>(metalness);
                    material->setSubMaterial(SubMaterialType::Metalness, subMaterial);
                }
                AssetManager::AddResource(material->getPath(), material);
                materials.push_back(material);
            }
            std::vector<std::shared_ptr<SubMesh>> submeshes = m_mesh->getSubmeshes();
            for (size_t i = 0; i < submeshes.size(); i++)
            {
                setSubmeshMaterial(i, materials[submeshes[i]->getMaterialIndex()]);
            }
        }
    }
}
