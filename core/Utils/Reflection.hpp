#pragma once
#include <string>
#include<memory>
#include<nlohmann/json.hpp>
class Object;

#include "reflang.hpp"
#include "C:/Users/Tort1k/Desktop/Tengine/core/ECS/Components/Transform.h"
#include "C:/Users/Tort1k/Desktop/Tengine/core/ECS/Components/Mesh.h"
#include "C:/Users/Tort1k/Desktop/Tengine/core/ECS/Components/Camera.h"

namespace reflang
{







	void Deserialize(nlohmann::json data, std::shared_ptr<::Object> object)
	{
		Transform::Deserialize(data,object);
		Mesh::Deserialize(data,object);
		Camera::Deserialize(data,object);
	}
}  // namespace reflang
