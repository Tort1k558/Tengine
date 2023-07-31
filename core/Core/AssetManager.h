#pragma once

#include<string>
#include<unordered_map>
#include<variant>
#include<filesystem>

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include"Renderer/Texture.h"
#include"Renderer/Shader.h"
#include"ECS/Components/Mesh.h"

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

	static std::shared_ptr<Shader> LoadShader(std::filesystem::path pathToVertexShader, std::filesystem::path pathToFragmentShader);
	static std::shared_ptr<Texture> LoadTexture(std::filesystem::path path);
	static std::shared_ptr<Mesh> LoadMesh(std::filesystem::path path);

	template<typename T>
	static std::shared_ptr<T> GetResource(std::filesystem::path path);
private:
	static std::string ReadFile(std::filesystem::path path);
	static std::unordered_map<std::string, Resource> m_resources;
	static std::shared_ptr<SubMesh> ProcessSubMesh(aiMesh* mesh, const aiScene* scene, std::filesystem::path directory);
	static void ProcessNode(std::shared_ptr<Mesh> mesh, aiNode* node, const aiScene* scene, std::filesystem::path directory);
	static std::shared_ptr<Texture> LoadMaterialTexture(aiMaterial* material, aiTextureType type, std::filesystem::path directory);
};

template<typename T>
inline std::shared_ptr<T> AssetManager::GetResource(std::filesystem::path path)
{
	std::shared_ptr<T>* resource = std::get_if<std::shared_ptr<T>>(&m_resources[path.string()]);
	if (!resource)
	{
		return nullptr;
	}
	return *resource;
}
