#pragma once

#include"ECS/Component.h"

namespace Tengine
{
	class Script : public Component
	{
	public:
		virtual ~Script() = default;
		virtual void start() {};
		virtual void update() {};
		virtual void serialize() {};
		virtual void deserialize() {};
	private:
		
	};
}