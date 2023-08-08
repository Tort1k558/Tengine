#pragma once
#include <string>
#include<memory>
#include<nlohmann/json.hpp>
class Object;

#include "reflang.hpp"
#include "C:/Users/Tort1k/Desktop/Tengine/core/ECS/Component.h"
#include "C:/Users/Tort1k/Desktop/Tengine/core/ECS/Components/Transform.h"
#include "C:/Users/Tort1k/Desktop/Tengine/core/ECS/Components/Mesh.h"
#include "C:/Users/Tort1k/Desktop/Tengine/core/ECS/Components/Camera.h"

namespace reflang
{









	void Deserialize(nlohmann::json data, std::shared_ptr<::Object> object)
	{
		Transform component0; 
		component0.deserialize(data, object); 
		Mesh component1; 
		component1.deserialize(data, object); 
		Camera component2; 
		component2.deserialize(data, object); 
		Component component3; 
		component3.deserialize(data, object); 
	}
}  // namespace reflang
