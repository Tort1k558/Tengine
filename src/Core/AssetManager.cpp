#include "AssetManager.h"

#include<fstream>
#include<sstream>
#include<assimp/Importer.hpp>
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
    UVec2 size;

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
    return texture;
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
