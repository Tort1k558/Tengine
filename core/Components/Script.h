#pragma once

#include"Scene/Component.h"

namespace Tengine
{
	class Script : public Component
	{
	public:
		virtual ~Script() = default;
		virtual void start() {};
		virtual void update() {};
	private:
		
	};
}