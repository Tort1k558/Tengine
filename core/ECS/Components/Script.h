#pragma once

#include"ECS/Component.h"

namespace Tengine
{
	class Script : public Component
	{
	public:
		virtual void start();
		virtual void update();
	private:

	};
}