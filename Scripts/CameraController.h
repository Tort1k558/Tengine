#pragma once

#include"Components/Script.h"
#include"SystemModule.h"

namespace Tengine
{
	class CameraController : public Script
	{
		SHOWINEDITOR;
	public:
		void start() override;
		void update() override;
		float speed = 7.0f;
		float cameraSensitivity = 0.314f;
		float dd;
	};
}