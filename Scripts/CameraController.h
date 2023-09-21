#pragma once

#include"Components/Script.h"

namespace Tengine
{
	class CameraController : public Script
	{
	public:
		void start() override;
		void update() override;
		DisplayInfo getDisplayInfo() override;
		float speed = 7.0f;
		float cameraSensitivity = 0.314f;
	};
}