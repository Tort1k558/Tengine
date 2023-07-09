#pragma once

#include<string>
#include<unordered_map>
#include<variant>
#include"Renderer/Shader.h"

class ResourceManager
{
public:
	ResourceManager() = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	using Resource = std::variant<std::shared_ptr<Shader>>;
	static std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string& pathToVertexShader, const std::string& pathToFragmentShader);
	template<typename T>
	static std::shared_ptr<T> GetResource(const std::string& name)
	{
		std::shared_ptr<T>* resource = std::get_if<std::shared_ptr<T>>(&m_resources[name]);
		return *resource;
	}
private:
	static std::string ReadFile(const std::string& path);
	static std::unordered_map<std::string, Resource> m_resources;
};