#include "AssetManager.h"

#include<fstream>
#include<sstream>
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
