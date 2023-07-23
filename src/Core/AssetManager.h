#pragma once

#include<string>
#include<unordered_map>
#include<variant>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include"Renderer/Texture.h"
#include"Renderer/Shader.h"
#include"Components/Mesh.h"

class AssetManager
{
public:
	AssetManager() = delete;
	AssetManager(const AssetManager&) = delete;
	AssetManager(AssetManager&&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;
	AssetManager& operator=(AssetManager&&) = delete;

	using Resource = std::variant<std::shared_ptr<Shader>,
								  std::shared_ptr<Texture>,
								  std::shared_ptr<Mesh>>;

	static std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string& pathToVertexShader, const std::string& pathToFragmentShader);
	static std::shared_ptr<Texture> LoadTexture(const std::string& name, const std::string& path);
	static std::shared_ptr<Mesh> LoadMesh(const std::string& name, const std::string& path);

	template<typename T>
	static std::shared_ptr<T> GetResource(const std::string& name);
private:
	static std::string ReadFile(const std::string& path);
	static std::unordered_map<std::string, Resource> m_resources;
	static SubMesh processSubMesh(aiMesh* mesh, const aiScene* scene);
	static void processNode(std::shared_ptr<Mesh> mesh, aiNode* node, const aiScene* scene);
};

template<typename T>
inline std::shared_ptr<T> AssetManager::GetResource(const std::string& name)
{
	std::shared_ptr<T>* resource = std::get_if<std::shared_ptr<T>>(&m_resources[name]);
	if (!resource)
	{
		return nullptr;
	}
	return *resource;
}
