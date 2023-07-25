#include "AssetManager.h"

#include<fstream>
#include<sstream>

#include<stb_image.h>

#include"Core/Logger.h"

std::unordered_map<std::string, AssetManager::Resource> AssetManager::m_resources;

std::shared_ptr<Shader> AssetManager::LoadShader(const std::string& name, const std::string& pathToVertexShader, const std::string& pathToFragmentShader)
{
    std::shared_ptr<Shader> shader = GetResource<Shader>(name);
    if (shader)
    {
        return shader;
    }
    shader = Shader::Create();
    shader->addShader(ReadFile(pathToVertexShader), ShaderType::VertexShader);
    shader->addShader(ReadFile(pathToFragmentShader), ShaderType::FragmentShader);
    shader->compile();
    m_resources[name] = shader;
    return shader;
}

std::shared_ptr<Texture> AssetManager::LoadTexture(const std::string& name, const std::string& path)
{
    std::shared_ptr<Texture> texture = GetResource<Texture>(name);
    if (texture)
    {
        return texture;
    }

    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data == nullptr)
    {
        Logger::Critical("ERROR::Failed to load the texture: {0}", path);
        return nullptr;
    }
    TextureType type = TextureType::RGB8;
    switch (channels)
    {
    case 3:
        type = TextureType::RGB8;
        break;
    case 4:
        type = TextureType::RGBA8;
        break;
    }
    texture = Texture::Create(data, { width,height }, type);
    stbi_image_free(data);

    m_resources[name] = texture;
    return texture;
}

std::shared_ptr<Mesh> AssetManager::LoadMesh(const std::string& name, const std::string& path)
{
    std::shared_ptr<Mesh> mesh = GetResource<Mesh>(name);
    if (mesh)
    {
        return mesh;
    }
    mesh = Component::Create<Mesh>();
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        Logger::Critical("ERROR::ASSIMP::{0}", importer.GetErrorString());
        return nullptr;
    }
    processNode(mesh,scene->mRootNode, scene);

    m_resources[name] = mesh;
    return std::shared_ptr<Mesh>(mesh);
}

std::string AssetManager::ReadFile(const std::string& path)
{
    std::ifstream file(path);
    if (file.is_open())
    {
        std::stringstream buffer;
        buffer << file.rdbuf();

        file.close();

        return buffer.str();
    }
    Logger::Critical("ERROR::Failed to open file::{0}", path);
    return "";
}

std::shared_ptr<SubMesh> AssetManager::processSubMesh(aiMesh* mesh, const aiScene* scene)
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
    return std::make_shared<SubMesh>(vertices, indices);
}

void AssetManager::processNode(std::shared_ptr<Mesh> mesh, aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
        mesh->addSubmesh(processSubMesh(aimesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(mesh, node->mChildren[i], scene);
    }
}
