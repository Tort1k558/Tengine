#include "SceneSerializer.h"

#include<nlohmann/json.hpp>

#include"Core/Logger.h"

void SceneSerializer::Serialize(std::shared_ptr<Scene> scene)
{
    nlohmann::json data;

    // ��������� ������ �������
    data["name"] = "John Doe";
    data["age"] = 30;
    data["is_student"] = true;

    // ������� JSON ������ �� �����
    Logger::Debug(data.dump(4));
}

std::shared_ptr<Scene> SceneSerializer::Deserialize(std::filesystem::path path)
{
	return std::shared_ptr<Scene>();
}
