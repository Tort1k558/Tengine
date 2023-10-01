#pragma once

#include<string>
#include<unordered_map>
#include<variant>
#include<filesystem>

#include"Core/Core.h"

struct aiMesh;
struct aiScene;
struct aiNode;
struct aiMaterial;
enum aiTextureType;

namespace Tengine
{
	class Texture;
	class Shader;
	class Mesh;
	class SubMesh;

	class TENGINE Resource
	{
	public:
		virtual ~Resource() = default;
		std::filesystem::path getPath() const;
		void setPath(std::filesystem::path path);
	protected:
		std::filesystem::path m_path;
	};

	class TENGINE AssetManager
	{
	public:
		AssetManager() = delete;

		static std::shared_ptr<Shader> LoadShader(std::filesystem::path pathToVertexShader, std::filesystem::path pathToFragmentShader);
		static std::shared_ptr<Texture> LoadTexture(std::filesystem::path path);
		static std::shared_ptr<Mesh> LoadMesh(std::filesystem::path path);
		static std::shared_ptr<Shader> AddShader(std::string_view name, std::string_view vertexSource, std::string_view fragmentSource);
		

		template<typename T>
		static std::shared_ptr<T> GetResource(std::string_view name);
	private:
		static std::string ReadFile(std::filesystem::path path);
		static std::shared_ptr<SubMesh> ProcessSubMesh(aiMesh* mesh, const aiScene* scene, std::filesystem::path directory);
		static void ProcessNode(std::shared_ptr<Mesh> mesh, aiNode* node, const aiScene* scene, std::filesystem::path directory);
		static std::shared_ptr<Texture> LoadMaterialTexture(aiMaterial* material, aiTextureType type, std::filesystem::path directory);

		static std::unordered_map<std::filesystem::path, std::shared_ptr<Resource>> m_resources;
	};

	template<typename T>
	inline std::shared_ptr<T> AssetManager::GetResource(std::string_view name)
	{
		if (m_resources.find(name) == m_resources.end())
		{
			return nullptr;
		}
		return std::dynamic_pointer_cast<T>(m_resources.at(name));
	}

}