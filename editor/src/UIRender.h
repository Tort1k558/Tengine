#pragma once

#include<string>
#include<vector>

#include<Core/Math.h>

namespace TengineEditor
{
	using namespace Tengine;

	class UIRender 
	{
	public:
		UIRender() = delete;

		static bool DrawFloat(std::string_view name, float* data, float speed = 1.0f, float min = 0.0f, float max = 0.0f);
		static bool DrawVec2(std::string_view name, float* data, float speed = 1.0f, float min = 0.0f, float max = 0.0f);
		static bool DrawVec3(std::string_view name, float* data, float speed = 1.0f, float min = 0.0f, float max = 0.0f);
		static bool DrawVec4(std::string_view name, float* data, float speed = 1.0f, float min = 0.0f, float max = 0.0f);

		static void DrawImage(void* id, Vec2 size);
		static bool DrawCombo(std::string_view name, std::vector<std::string> elements, int& currentElement);
		static bool DrawButton(std::string_view name);
		static bool DrawCheckbox(std::string_view name, bool* data);
		static bool DrawCollapsingHeader(std::string_view name);
		static bool DrawFile(std::string_view name, std::string& pathToFile);
	};
}