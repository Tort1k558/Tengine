#pragma once

#include<string>
#include<unordered_map>
#include<variant>
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

	static std::shared_ptr<Shader> LoadShader(std::string_view pathToVertexShader, std::string_view pathToFragmentShader);
	static std::shared_ptr<Texture> LoadTexture(std::string_view path);
	static std::shared_ptr<Mesh> LoadMesh(std::string_view path);

	template<typename T>
	static std::shared_ptr<T> GetResource(std::string_view path);
private:
	static std::string ReadFile(std::string_view path);
	static std::unordered_map<std::string, Resource> m_resources;
	static std::shared_ptr<SubMesh> ProcessSubMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory);
	static void ProcessNode(std::shared_ptr<Mesh> mesh, aiNode* node, const aiScene* scene, const std::string& directory);
	static std::shared_ptr<Texture> LoadMaterialTexture(aiMaterial* material, aiTextureType type, const std::string& directory);
};

template<typename T>
inline std::shared_ptr<T> AssetManager::GetResource(std::string_view path)
{
	std::shared_ptr<T>* resource = std::get_if<std::shared_ptr<T>>(&m_resources[path.data()]);
	if (!resource)
	{
		return nullptr;
	}
	return *resource;
}
