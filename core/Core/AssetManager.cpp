#include "AssetManager.h"

#include<fstream>
#include<sstream>

#include<stb_image.h>

#include<assimp/Importer.hpp>
#include<assimp/postprocess.h>
#include<assimp/scene.h>

#include"Components/Model.h"
#include"Core/Logger.h"
#include"Renderer/Shader.h"
#include"Renderer/Texture.h"
#include"Renderer/CubeMapTexture.h"
#include"Utils/Mesh.h"
#include"Utils/Primitives.h"
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

    std::shared_ptr<SubMesh> ProcessSubMesh(aiMesh* mesh, const aiScene* scene)
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
        if (mesh->mMaterialIndex >= 0)
        {
            submesh->setMaterialIndex(mesh->mMaterialIndex);
        }
        return submesh;
    }

    void ProcessNode(std::shared_ptr<Mesh> mesh, aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
            mesh->addSubmesh(ProcessSubMesh(aimesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(mesh, node->mChildren[i], scene);
        }
    }

    std::unordered_map<std::string, std::shared_ptr<Resource>> AssetManager::m_resources;

    std::shared_ptr<Shader> AssetManager::LoadShader(std::string_view name, std::filesystem::path pathToVertexShader, std::filesystem::path pathToFragmentShader)
    {
        std::shared_ptr<Shader> shader = GetResource<Shader>(name);
        if (shader)
        {
            return shader;
        }
        shader = Shader::Create();
        shader->addShader(ReadFile(pathToVertexShader), ShaderType::Vertex);
        shader->addShader(ReadFile(pathToFragmentShader), ShaderType::Fragment);
        shader->compile();
        m_resources[name.data()] = shader;

        return shader;
    }

    std::shared_ptr<Texture> AssetManager::LoadTexture(std::filesystem::path path)
    {
        std::shared_ptr<Texture> texture = GetResource<Texture>(path.string());
        if (texture)
        {
            return texture;
        }
        std::shared_ptr<Image> image = LoadImage(path);

        TextureType type = TextureType::RGB8;
        switch (image->getChannels())
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
        texture = Texture::Create(image, type);
        texture->setPath(path);

        m_resources[path.string()] = texture;
        return texture;
    }


    std::shared_ptr<Material> AssetManager::LoadMaterial(std::filesystem::path path)
    {
        std::shared_ptr<Material> material = GetResource<Material>(path.string());
        if (material)
        {
            return material;
        }
        material = std::make_shared<Material>();
        material->setPath(path);
        std::ifstream file(path.string());
        if (file.is_open())
        {
            nlohmann::json data = nlohmann::json::parse(file);
            auto loadSubMaterial = [&material](nlohmann::json& data, SubMaterialType type)
                {
                    std::shared_ptr<SubMaterial> subMaterial = std::make_shared<SubMaterial>();
                    if (data.find("path") != data.end())
                    {
                        std::string pathToTexture = data["path"].get<std::string>();
                        if (pathToTexture != "null")
                        {
                            subMaterial->setTexture(LoadTexture(pathToTexture));
                        }
                    }
                    Vec3 color = { data["color"][0],data["color"][1],data["color"][2] };
                    subMaterial->setColor(color);
                    material->setSubMaterial(type, subMaterial);
                };
            for (const auto& item : data.items())
            {
                if (item.key() == "Diffuse")
                {
                    loadSubMaterial(item.value(), SubMaterialType::Diffuse);
                }
                else if (item.key() == "Normal")
                {
                    loadSubMaterial(item.value(), SubMaterialType::Normal);
                }
                else if (item.key() == "Specular")
                {
                    loadSubMaterial(item.value(), SubMaterialType::Specular);
                }
                else if (item.key() == "Roughness")
                {
                    loadSubMaterial(item.value(), SubMaterialType::Roughness);
                }
            }
            file.close();
        }
        else
        {
            Logger::Critical("ERROR::AssetManager::Failed to open the material file!");
        }
        m_resources[path.string()] = material;
        return material;
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
        if (path.string().find("Primitive") != std::string::npos)
        {
            if (path == "Primitive::Quad")
            {
                mesh = Primitives::CreateQuad();

            }
            else if (path == "Primitive::Cube")
            {
                mesh = Primitives::CreateCube();
            }
            else if (path.string().find("Primitive::Sphere::") != std::string::npos)
            {
                size_t sectorsPos = path.string().find("::Sectors::");
                size_t stacksPos = path.string().rfind("::Stacks::");

                int sectors;
                int stacks;

                if (sectorsPos == std::string::npos || stacksPos == std::string::npos || sectorsPos == stacksPos) 
                {
                    sectors = 1;
                    stacks = 1;
                }
                sectors = std::stoi(path.string().substr(sectorsPos + 11, stacksPos - sectorsPos - 11));
                stacks = std::stoi(path.string().substr(stacksPos + 10));
                mesh = Primitives::CreateSphere(sectors, stacks);
            }
        }
        else
        {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path.string().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                Logger::Critical("ERROR::ASSIMP::{0}", importer.GetErrorString());
                return nullptr;
            }
            ProcessNode(mesh, scene->mRootNode, scene);
        }
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

    std::shared_ptr<Model> AssetManager::LoadModel(std::filesystem::path path)
    {
        std::shared_ptr<Model> model = GetResource<Model>(path.string());
        if (model)
        {
            return model;
        }

        model = Component::Create<Model>();
        model->setPath(path);
        std::ifstream file(path.string());
        if (file.is_open())
        {
            nlohmann::json data = nlohmann::json::parse(file);
            std::string pathToMesh = data["PathToMesh"].get<std::string>();
            if (!pathToMesh.empty())
            {
                model->setMesh(LoadMesh(pathToMesh));
                std::vector<std::shared_ptr<SubMesh>> submeshes = model->getMesh()->getSubmeshes();
                for (const auto& item : data.items())
                {
                    if (item.key().find("PathToSubmeshMaterial") != std::string::npos)
                    {
                        int submeshNumber = std::stoi(item.key().substr(item.key().find_last_of("PathToSubmeshMaterial") + 1,
                            item.key().size() - item.key().find_last_of("PathToSubmeshMaterial")));
                        model->setSubmeshMaterial(submeshNumber, LoadMaterial(item.value().get<std::string>()));
                    }
                }
            }
        }
        m_resources[path.string()] = model;
        return model;
    }

    std::shared_ptr<Model> AssetManager::CreateModel(std::filesystem::path pathToMesh)
    {
        std::shared_ptr<Model> model = Component::Create<Model>();

        std::shared_ptr<Mesh> mesh = LoadMesh(pathToMesh);
        model->setMesh(mesh);

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(pathToMesh.string().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            Logger::Critical("ERROR::ASSIMP::{0}", importer.GetErrorString());
            return nullptr;
        }
        std::filesystem::path parentPath = pathToMesh.parent_path();
        std::vector<std::shared_ptr<Material>> materials;
        for (size_t i = 0; i < scene->mNumMaterials; i++)
        {
            std::shared_ptr<Material> material = std::make_shared<Material>();
            for (size_t i = 0; true; i++)
            {
                if (!std::filesystem::exists(parentPath.string() + "/Material" + std::to_string(i) + ".material"))
                {
                    material->setPath(parentPath.string() + "/Material" + std::to_string(i) + ".material");
                    break;
                }
            }
            aiMaterial* aimaterial = scene->mMaterials[i]; std::shared_ptr<Texture> diffuse = LoadMaterialTexture(aimaterial, aiTextureType_DIFFUSE, parentPath);
            if (diffuse)
            {
                std::shared_ptr subMaterial = std::make_shared<SubMaterial>(diffuse);
                material->setSubMaterial(SubMaterialType::Diffuse, subMaterial);
            }
            std::shared_ptr<Texture> normals = LoadMaterialTexture(aimaterial, aiTextureType_NORMALS, parentPath);
            if (normals)
            {
                std::shared_ptr subMaterial = std::make_shared<SubMaterial>(normals);
                material->setSubMaterial(SubMaterialType::Normal, subMaterial);
            }
            std::shared_ptr<Texture> specular = LoadMaterialTexture(aimaterial, aiTextureType_SPECULAR, parentPath);
            if (specular)
            {
                std::shared_ptr subMaterial = std::make_shared<SubMaterial>(specular);
                material->setSubMaterial(SubMaterialType::Specular, subMaterial);
            }
            std::shared_ptr<Texture> roughness = LoadMaterialTexture(aimaterial, aiTextureType_DIFFUSE_ROUGHNESS, parentPath);
            if (roughness)
            {
                std::shared_ptr subMaterial = std::make_shared<SubMaterial>(roughness);
                material->setSubMaterial(SubMaterialType::Roughness, subMaterial);
            }
            std::shared_ptr<Texture> occlusion = LoadMaterialTexture(aimaterial, aiTextureType_AMBIENT_OCCLUSION, parentPath);
            if (occlusion)
            {
                std::shared_ptr subMaterial = std::make_shared<SubMaterial>(occlusion);
                material->setSubMaterial(SubMaterialType::Occlusion, subMaterial);
            }
            std::shared_ptr<Texture> metalness = LoadMaterialTexture(aimaterial, aiTextureType_METALNESS, parentPath);
            if (metalness)
            {
                std::shared_ptr subMaterial = std::make_shared<SubMaterial>(metalness);
                material->setSubMaterial(SubMaterialType::Metalness, subMaterial);
            }
            m_resources[material->getPath().string()] = material;
            materials.push_back(material);
        }
        std::vector<std::shared_ptr<SubMesh>> submeshes = mesh->getSubmeshes();
        for (size_t i = 0; i < submeshes.size(); i++)
        {
            model->setSubmeshMaterial(i, materials[submeshes[i]->getMaterialIndex()]);
        }

        return model;
    }

    std::shared_ptr<CubeMapTexture> AssetManager::CreateCubeMapTexture(std::array<std::filesystem::path, 6> paths)
    {
        std::filesystem::path parentPath = paths[0].parent_path();
        std::filesystem::path path;
        for (size_t i = 0; true; i++)
        {
            if (!std::filesystem::exists(parentPath.string() + "/CubeMap" + std::to_string(i) + ".cubeMap"))
            {
                path = parentPath.string() + "/CubeMap" + std::to_string(i) + ".cubeMap";
                break;
            }
        }
        std::array<std::shared_ptr<Texture>, 6> textures;
        for (size_t i = 0; i < paths.size(); i++)
        {
            textures[i] = LoadTexture(paths[i]);
        }
        std::shared_ptr<CubeMapTexture> cubeMapTexture = CubeMapTexture::Create(textures);
        cubeMapTexture->setSupportingInfo("right", textures[0]->getPath().string());
        cubeMapTexture->setSupportingInfo("left", textures[1]->getPath().string());
        cubeMapTexture->setSupportingInfo("top", textures[2]->getPath().string());
        cubeMapTexture->setSupportingInfo("bottom", textures[3]->getPath().string());
        cubeMapTexture->setSupportingInfo("front", textures[4]->getPath().string());
        cubeMapTexture->setSupportingInfo("back", textures[5]->getPath().string());
        cubeMapTexture->setPath(path);
        m_resources[path.string()] = cubeMapTexture;
        SaveCubeMapTexture(cubeMapTexture.get());
        return cubeMapTexture;
    }

    std::shared_ptr<CubeMapTexture> AssetManager::LoadCubeMapTexture(std::filesystem::path path)
    {
        std::shared_ptr<CubeMapTexture> cubeMapTexture = GetResource<CubeMapTexture>(path.string());
        if (cubeMapTexture)
        {
            return cubeMapTexture;
        }
        std::array<std::shared_ptr<Texture>, 6> textures;
        std::ifstream file(path.string());
        if (file.is_open())
        {
            nlohmann::json data = nlohmann::json::parse(file);
            for (const auto& item : data.items())
            {
                if (item.key() == "right")
                {
                    textures[0] = LoadTexture(item.value());
                }
                else if (item.key() == "left")
                {
                    textures[1] = LoadTexture(item.value());
                }
                else if (item.key() == "top")
                {
                    textures[2] = LoadTexture(item.value());
                }
                else if (item.key() == "bottom")
                {
                    textures[3] = LoadTexture(item.value());
                }
                else if (item.key() == "front")
                {
                    textures[4] = LoadTexture(item.value());
                }
                else if (item.key() == "back")
                {
                    textures[5] = LoadTexture(item.value());
                }
            }
            file.close();
        }
        else
        {
            Logger::Critical("ERROR::AssetManager::Failed to open the material file!");
        }
        cubeMapTexture = CubeMapTexture::Create(textures);
        cubeMapTexture->setPath(path);
        for (size_t i = 0; i < textures.size(); i++)
        {
            cubeMapTexture->setSupportingInfo("right", textures[0]->getPath().string());
            cubeMapTexture->setSupportingInfo("left", textures[1]->getPath().string());
            cubeMapTexture->setSupportingInfo("top", textures[2]->getPath().string());
            cubeMapTexture->setSupportingInfo("bottom", textures[3]->getPath().string());
            cubeMapTexture->setSupportingInfo("front", textures[4]->getPath().string());
            cubeMapTexture->setSupportingInfo("back", textures[5]->getPath().string());
        }
        m_resources[path.string()] = cubeMapTexture;
;       return cubeMapTexture;
    }

    void AssetManager::SaveModel(Model* model)
    {
        nlohmann::json data;
        if (model->getMesh())
        {
            data["PathToMesh"] = model->getMesh()->getPath();
            std::vector<std::shared_ptr<SubMesh>> submeshes = model->getMesh()->getSubmeshes();
            for (size_t i = 0; i < submeshes.size(); i++)
            {
                if (model->hasSubmeshMaterial(i))
                {
                    data["PathToSubmeshMaterial" + std::to_string(i)] = model->getSubmeshMaterial(i)->getPath();
                }
            }
        }
        else
        {
            data["PathToMesh"] = "";
        }
        std::string pathToModelFile;
        if (model->getPath().empty())
        {
            if (model->getMesh()->getPath().string().find("Primitive::") != std::string::npos)
            {
                size_t i = 0;
                while (std::filesystem::exists("Primitive" + std::to_string(i) + ".model"))
                {
                    i++;
                }
                pathToModelFile = "Primitive" + std::to_string(i) + ".model";
            }
            else
            {
                pathToModelFile = model->getMesh()->getPath().parent_path().string() + "/" + model->getMesh()->getPath().stem().string() + ".model";
            }
        }
        else
        {
            pathToModelFile = model->getPath().parent_path().string() + "/" + model->getPath().stem().string() + ".model";
        }

        model->setPath(pathToModelFile);
        std::ofstream file(pathToModelFile, std::ios_base::out);
        if (file.is_open())
        {
            file << data.dump(4);
            file.close();
        }
        else
        {
            Logger::Critical("ERROR::Model::Failed to save the model file!");
        }
    }

    void AssetManager::SaveMaterial(Material* material)
    {
        if (!material->getPath().empty())
        {
            nlohmann::json data;
            for (const auto& subMaterial : material->getSubMaterials())
            {
                std::string subMaterialType;
                switch (subMaterial.first)
                {
                case SubMaterialType::Diffuse:
                    subMaterialType = "Diffuse";
                    break;
                case SubMaterialType::Normal:
                    subMaterialType = "Normal";
                    break;
                case SubMaterialType::Specular:
                    subMaterialType = "Specular";
                    break;
                case SubMaterialType::Height:
                    subMaterialType = "Height";
                    break;
                case SubMaterialType::Roughness:
                    subMaterialType = "Roughness";
                    break;
                default:
                    break;
                }
                if (!subMaterialType.empty())
                {
                    if (subMaterial.second->hasTexture())
                    {
                        data[subMaterialType]["path"] = subMaterial.second->getTexture()->getPath();
                    }
                    Vec3 color = subMaterial.second->getColor();
                    data[subMaterialType]["color"] = { color.r , color.g, color.b };
                }
            }
            std::ofstream file(material->getPath().parent_path().string() + "/" + material->getPath().stem().string() + ".material", std::ios_base::out);
            if (file.is_open()) {
                file << data.dump(4);
                file.close();
            }
            else {
                Logger::Critical("ERROR::Material::Failed to save the material!");
            }
        }
    }

    void AssetManager::SaveCubeMapTexture(CubeMapTexture* texture)
    {
        if (!texture->getPath().empty())
        {
            nlohmann::json data;
            data["right"] = texture->getSupportingInfo("right");
            data["left"] = texture->getSupportingInfo("left");
            data["top"] = texture->getSupportingInfo("top");
            data["bottom"] = texture->getSupportingInfo("bottom");
            data["front"] = texture->getSupportingInfo("front");
            data["back"] = texture->getSupportingInfo("back");

            std::ofstream file(texture->getPath().parent_path().string() + "/" + texture->getPath().stem().string() + ".cubeMap", std::ios_base::out);
            if (file.is_open()) {
                file << data.dump(4);
                file.close();
            }
            else {
                Logger::Critical("ERROR::Material::Failed to save the cubeMap texture!");
            }
        }
    }

    std::shared_ptr<Image> AssetManager::LoadImage(std::filesystem::path path, bool flipY)
    {
        stbi_set_flip_vertically_on_load(flipY);
        int width, height, channels;

        unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
        if (data == nullptr)
        {
            Logger::Critical("ERROR::Failed to load the image: {0}", path.string().c_str());
            return nullptr;
        }
        std::shared_ptr<Image> image = std::make_shared<Image>(data, UVec2(width, height), channels);
        image->setPath(path);
        return image;
    }

    std::filesystem::path Resource::getPath() const
    {
        return m_path;
    }

    void Resource::setPath(std::filesystem::path path)
    {
        m_path = path;
    }

    void Resource::setSupportingInfo(std::string_view key, std::string_view value)
    {
        m_supportingInfo[key.data()] = value;
    }

    std::string Resource::getSupportingInfo(std::string_view key)
    {
        if (m_supportingInfo.find(key.data()) != m_supportingInfo.end())
        {
            return m_supportingInfo[key.data()];
        }
        return "";
    }
}