#include "AssetManager.h"

#include<fstream>
#include<sstream>

#include<stb_image.h>

#include<assimp/Importer.hpp>
#include<assimp/postprocess.h>
#include<assimp/scene.h>

#include"Components/Model.h"
#include"Core/Logger.h"
#include"Renderer/Shader.h"
#include"Renderer/Texture.h"
#include"Utils/Mesh.h"
#include"Utils/Primitives.h"

namespace Tengine
{
    std::string ReadFile(std::filesystem::path path)
    {
        std::ifstream file(path);
        if (file.is_open())
        {
            std::stringstream buffer;
            buffer << file.rdbuf();

            file.close();

            return buffer.str();
        }
        Logger::Critical("ERROR::Failed to open file::{0}", path.string().c_str());
        return "";
    }

    std::shared_ptr<Texture> LoadMaterialTexture(aiMaterial* material, aiTextureType type, std::filesystem::path directory)
    {
        if (material->GetTextureCount(type) > 0)
        {
            aiString pathToTexture;
            material->GetTexture(type, 0, &pathToTexture);
            return AssetManager::LoadTexture(directory.string() + "/" + pathToTexture.C_Str());
        }
        return nullptr;
    }

    std::shared_ptr<SubMesh> ProcessSubMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        for (size_t i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;

            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
            if (mesh->HasTextureCoords(0))
            {
                vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
            }
            else
            {
                vertex.texCoords = Vec2(0.0f, 0.0f);
            }
            vertices.push_back(vertex);
        }

        std::vector<unsigned int> indices;
        for (size_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (size_t j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }
        std::shared_ptr<SubMesh> submesh = std::make_shared<SubMesh>(vertices, indices);
        if (mesh->mMaterialIndex >= 0)
        {
            submesh->setMaterialIndex(mesh->mMaterialIndex);
        }
        return submesh;
    }

    void ProcessNode(std::shared_ptr<Mesh> mesh, aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
            mesh->addSubmesh(ProcessSubMesh(aimesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(mesh, node->mChildren[i], scene);
        }
    }

    std::unordered_map<std::string, std::shared_ptr<Resource>> AssetManager::m_resources;

    std::shared_ptr<Shader> AssetManager::LoadShader(std::string_view name, std::filesystem::path pathToVertexShader, std::filesystem::path pathToFragmentShader)
    {
        std::shared_ptr<Shader> shader = GetResource<Shader>(name);
        if (shader)
        {
            return shader;
        }
        shader = Shader::Create();
        shader->addShader(ReadFile(pathToVertexShader), ShaderType::Vertex);
        shader->addShader(ReadFile(pathToFragmentShader), ShaderType::Fragment);
        shader->compile();
        m_resources[name.data()] = shader;

        return shader;
    }

    std::shared_ptr<Texture> AssetManager::LoadTexture(std::filesystem::path path)
    {
        std::shared_ptr<Texture> texture = GetResource<Texture>(path.string());
        if (texture)
        {
            return texture;
        }
        stbi_set_flip_vertically_on_load(true);
        int width, height, channels;

        unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
        if (data == nullptr)
        {
            Logger::Critical("ERROR::Failed to load the texture: {0}", path.string().c_str());
            return nullptr;
        }
        TextureType type = TextureType::RGB8;
        switch (channels)
        {
        case 1:
            type = TextureType::R8;
            break;
        case 2:
            type = TextureType::RG8;
            break;
        case 3:
            type = TextureType::RGB8;
            break;
        case 4:
            type = TextureType::RGBA8;
            break;
        }
        texture = Texture::Create(data, { width,height }, type);
        texture->setPath(path);
        stbi_image_free(data);

        m_resources[path.string()] = texture;
        return texture;
    }

    std::shared_ptr<Material> AssetManager::LoadMaterial(std::filesystem::path path)
    {
        std::shared_ptr<Material> material = GetResource<Material>(path.string());
        if (material)
        {
            return material;
        }
        material = std::make_shared<Material>();
        material->setPath(path);
        std::ifstream file(path.string());
        if (file.is_open())
        {
            nlohmann::json data = nlohmann::json::parse(file);
            for (const auto& item : data.items())
            {
                if (item.key() == "Diffuse")
                {
                    material->setTextureMaterial(MaterialTexture::Diffuse, LoadTexture(item.value().get<std::string>()));
                }
                else if (item.key() == "Normal")
                {
                    material->setTextureMaterial(MaterialTexture::Normal, LoadTexture(item.value().get<std::string>()));
                }
                else if (item.key() == "Specular")
                {
                    material->setTextureMaterial(MaterialTexture::Specular, LoadTexture(item.value().get<std::string>()));
                }
            }
            file.close();
        }
        else
        {
            Logger::Critical("ERROR::AssetManager::Failed to open the material file!");
        }
        m_resources[path.string()] = material;
        return material;
    }

    std::shared_ptr<Mesh> AssetManager::LoadMesh(std::filesystem::path path)
    {
        std::shared_ptr<Mesh> mesh = GetResource<Mesh>(path.string());
        if (mesh)
        {
            return mesh;
        }

        mesh = Component::Create<Mesh>();
        mesh->setPath(path);
        if (path.string().find("Primitive") != std::string::npos)
        {
            if (path == "Primitive::Quad")
            {
                mesh = Primitives::CreateQuad();

            }
            else if (path == "Primitive::Cube")
            {
                mesh = Primitives::CreateCube();
            }
            else if (path.string().find("Primitive::Sphere::") != std::string::npos)
            {
                size_t sectorsPos = path.string().find("::Sectors::");
                size_t stacksPos = path.string().rfind("::Stacks::");

                int sectors;
                int stacks;

                if (sectorsPos == std::string::npos || stacksPos == std::string::npos || sectorsPos == stacksPos) 
                {
                    sectors = 1;
                    stacks = 1;
                }
                sectors = std::stoi(path.string().substr(sectorsPos + 11, stacksPos - sectorsPos - 11));
                stacks = std::stoi(path.string().substr(stacksPos + 10));
                mesh = Primitives::CreateSphere(sectors, stacks);
            }
        }
        else
        {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path.string().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                Logger::Critical("ERROR::ASSIMP::{0}", importer.GetErrorString());
                return nullptr;
            }
            ProcessNode(mesh, scene->mRootNode, scene);
        }
        m_resources[path.string()] = std::dynamic_pointer_cast<Resource>(std::make_shared<Mesh>(*mesh));
        return mesh;
    }

    std::shared_ptr<Shader> AssetManager::AddShader(std::string_view name,std::string_view vertexSource, std::string_view fragmentSource)
    {
        std::shared_ptr<Shader> shader = Shader::Create();
        shader->addShader(vertexSource, ShaderType::Vertex);
        shader->addShader(fragmentSource, ShaderType::Fragment);
        shader->compile();
        m_resources[name.data()] = shader;

        return shader;
    }

    std::shared_ptr<Model> AssetManager::LoadModel(std::filesystem::path path)
    {
        std::shared_ptr<Model> model = GetResource<Model>(path.string());
        if (model)
        {
            return model;
        }

        model = Component::Create<Model>();
        std::ifstream file(path.string());
        if (file.is_open())
        {
            nlohmann::json data = nlohmann::json::parse(file);
            std::string pathToMesh = data["PathToMesh"].get<std::string>();
            model->setMesh(LoadMesh(pathToMesh));
            std::vector<std::shared_ptr<SubMesh>> submeshes = model->getMesh()->getSubmeshes();
            for (const auto& item : data.items())
            {
                if (item.key().find("PathToSubmeshMaterial") != std::string::npos)
                {
                    int submeshNumber = std::stoi(item.key().substr(item.key().find_last_of("PathToSubmeshMaterial") + 1,
                        item.key().size() - item.key().find_last_of("PathToSubmeshMaterial")));
                    model->setSubmeshMaterial(submeshNumber, LoadMaterial(item.value().get<std::string>()));
                }
            }
        }
        m_resources[path.string()] = model;
        return model;
    }

    std::shared_ptr<Model> AssetManager::CreateModel(std::filesystem::path pathToMesh)
    {
        std::shared_ptr<Model> model = Component::Create<Model>();

        std::shared_ptr<Mesh> mesh = LoadMesh(pathToMesh);
        model->setMesh(mesh);

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(pathToMesh.string().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            Logger::Critical("ERROR::ASSIMP::{0}", importer.GetErrorString());
            return nullptr;
        }
        std::filesystem::path parentPath = pathToMesh.parent_path();
        std::vector<std::shared_ptr<Material>> materials;
        for (size_t i = 0; i < scene->mNumMaterials; i++)
        {
            std::shared_ptr<Material> material = std::make_shared<Material>();
            for (size_t i = 0; true; i++)
            {
                if (!std::filesystem::exists(parentPath.string() + "/Material" + std::to_string(i) + ".material"))
                {
                    material->setPath(parentPath.string() + "/Material" + std::to_string(i) + ".material");
                    break;
                }
            }
            aiMaterial* aimaterial = scene->mMaterials[i]; std::shared_ptr<Texture> diffuse = LoadMaterialTexture(aimaterial, aiTextureType_DIFFUSE, parentPath);
            if (diffuse)
            {
                material->setTextureMaterial(MaterialTexture::Diffuse, diffuse);
            }
            std::shared_ptr<Texture> specular = LoadMaterialTexture(aimaterial, aiTextureType_SPECULAR, parentPath);
            if (specular)
            {
                material->setTextureMaterial(MaterialTexture::Specular, specular);
            }

            std::shared_ptr<Texture> normals = LoadMaterialTexture(aimaterial, aiTextureType_NORMALS, parentPath);
            if (normals)
            {
                material->setTextureMaterial(MaterialTexture::Normal, normals);
            }
            std::shared_ptr<Texture> roughness = LoadMaterialTexture(aimaterial, aiTextureType_DIFFUSE_ROUGHNESS, parentPath);
            if (roughness)
            {
                material->setTextureMaterial(MaterialTexture::Roughness, roughness);
            }
            std::shared_ptr<Texture> occlusion = LoadMaterialTexture(aimaterial, aiTextureType_AMBIENT_OCCLUSION, parentPath);
            if (occlusion)
            {
                material->setTextureMaterial(MaterialTexture::Occlusion, occlusion);
            }
            std::shared_ptr<Texture> metalness = LoadMaterialTexture(aimaterial, aiTextureType_METALNESS, parentPath);
            if (metalness)
            {
                material->setTextureMaterial(MaterialTexture::Metalness, metalness);
            }
            m_resources[material->getPath().string()] = material;
            materials.push_back(material);
        }
        std::vector<std::shared_ptr<SubMesh>> submeshes = mesh->getSubmeshes();
        for (size_t i = 0; i < submeshes.size(); i++)
        {
            model->setSubmeshMaterial(i, materials[submeshes[i]->getMaterialIndex()]);
        }

        return model;
    }

    std::filesystem::path Resource::getPath() const
    {
        return m_path;
    }

    void Resource::setPath(std::filesystem::path path)
    {
        m_path = path;
    }
}