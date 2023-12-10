#pragma once

#include"Components/Script.h"

namespace Tengine
{
	class CameraController : public Script
	{
		SERIALIZABLE;
	public:
		void start() override;
		void update() override;
		float speed = 7.0f;
		float cameraSensitivity = 0.314f;
	};
}