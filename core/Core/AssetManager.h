#pragma once

#include<string>
#include<unordered_map>
#include<variant>
#include<filesystem>

#include"Core/Core.h"

namespace Tengine
{
	class Texture;
	class Shader;
	class Mesh;
	class SubMesh;
	class Material;
	class Model;

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

		static std::shared_ptr<Shader> LoadShader(std::string_view name, std::filesystem::path pathToVertexShader, std::filesystem::path pathToFragmentShader);
		static std::shared_ptr<Texture> LoadTexture(std::filesystem::path path);
		static std::shared_ptr<Material> LoadMaterial(std::filesystem::path path);
		static std::shared_ptr<Mesh> LoadMesh(std::filesystem::path path);
		static std::shared_ptr<Shader> AddShader(std::string_view name, std::string_view vertexSource, std::string_view fragmentSource);
		
		static std::shared_ptr<Model> LoadModel(std::filesystem::path path);
		static std::shared_ptr<Model> CreateModel(std::filesystem::path pathToMesh);

		template<typename T>
		static std::shared_ptr<T> GetResource(std::string_view name);
	private:

		static std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;
	};

	template<typename T>
	inline std::shared_ptr<T> AssetManager::GetResource(std::string_view name)
	{
		if (m_resources.find(name.data()) == m_resources.end())
		{
			return nullptr;
		}
		return std::dynamic_pointer_cast<T>(m_resources.at(name.data()));
	}

}