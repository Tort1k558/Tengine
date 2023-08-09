#pragma once
#include <string>
#include<memory>
#include<nlohmann/json.hpp>

class Tengine::Object;

#include "reflang.hpp"

namespace reflang
{

	void Deserialize(nlohmann::json data, std::shared_ptr<Tengine::Object> object)
	{
	}
}  // namespace reflang
