#include "AssetManager.h"

#include<fstream>
#include<sstream>

#include<stb_image.h>

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include"Core/Logger.h"
#include"Renderer/Shader.h"
#include"Renderer/Texture.h"
#include"Components/Mesh.h"

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

    std::shared_ptr<SubMesh> ProcessSubMesh(aiMesh* mesh, const aiScene* scene, std::filesystem::path directory)
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
        std::shared_ptr<Material> material = std::make_shared<Material>();
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* aimaterial = scene->mMaterials[mesh->mMaterialIndex];
            std::shared_ptr<Texture> diffuse = LoadMaterialTexture(aimaterial, aiTextureType_DIFFUSE, directory);
            if (diffuse)
            {
                material->setTextureMaterial(MaterialTexture::Diffuse, diffuse);
            }
            std::shared_ptr<Texture> specular = LoadMaterialTexture(aimaterial, aiTextureType_SPECULAR, directory);
            if (specular)
            {
                material->setTextureMaterial(MaterialTexture::Specular, specular);
            }
            std::shared_ptr<Texture> normals = LoadMaterialTexture(aimaterial, aiTextureType_NORMALS, directory);
            if (normals)
            {
                material->setTextureMaterial(MaterialTexture::Normal, normals);
            }
            std::shared_ptr<Texture> roughness = LoadMaterialTexture(aimaterial, aiTextureType_DIFFUSE_ROUGHNESS, directory);
            if (roughness)
            {
                material->setTextureMaterial(MaterialTexture::Roughness, roughness);
            }
            std::shared_ptr<Texture> occlusion = LoadMaterialTexture(aimaterial, aiTextureType_AMBIENT_OCCLUSION, directory);
            if (occlusion)
            {
                material->setTextureMaterial(MaterialTexture::Occlusion, occlusion);
            }
        }
        submesh->setMaterial(material);
        return submesh;
    }

    void ProcessNode(std::shared_ptr<Mesh> mesh, aiNode* node, const aiScene* scene, std::filesystem::path directory)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
            mesh->addSubmesh(ProcessSubMesh(aimesh, scene, directory));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(mesh, node->mChildren[i], scene, directory);
        }
    }

    std::unordered_map<std::string, std::shared_ptr<Resource>> AssetManager::m_resources;

    std::shared_ptr<Shader> AssetManager::LoadShader(std::filesystem::path pathToVertexShader, std::filesystem::path pathToFragmentShader)
    {
        std::shared_ptr<Shader> shader = GetResource<Shader>(pathToVertexShader.string() + pathToFragmentShader.string());
        if (shader)
        {
            return shader;
        }
        shader = Shader::Create();
        shader->addShader(ReadFile(pathToVertexShader), ShaderType::Vertex);
        shader->addShader(ReadFile(pathToFragmentShader), ShaderType::Fragment);
        shader->compile();
        m_resources[pathToVertexShader.string() + pathToFragmentShader.string()] = shader;

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

    std::shared_ptr<Mesh> AssetManager::LoadMesh(std::filesystem::path path)
    {
        std::shared_ptr<Mesh> mesh = GetResource<Mesh>(path.string());
        if (mesh)
        {
            return mesh;
        }
        mesh = Component::Create<Mesh>();
        mesh->setPath(path);
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path.string().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            Logger::Critical("ERROR::ASSIMP::{0}", importer.GetErrorString());
            return nullptr;
        }
        ProcessNode(mesh, scene->mRootNode, scene, path.parent_path());
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

    std::filesystem::path Resource::getPath() const
    {
        return m_path;
    }

    void Resource::setPath(std::filesystem::path path)
    {
        m_path = path;
    }
}