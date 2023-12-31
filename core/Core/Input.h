#pragma once

#include<unordered_map>

#include"Core/Keys.h"
#include"Core/Math.h"
#include"Core/Core.h"

namespace Tengine
{

	class TENGINE Input
	{
	public:
		Input() = delete;
		Input(const Input&) = delete;
		Input(Input&&) = delete;
		Input& operator=(const Input&) = delete;
		Input& operator=(Input&&) = delete;

		static bool IsKeyPressed(KeyCode code);
		static void PressKey(KeyCode code);
		static void ReleaseKey(KeyCode code);

		static bool IsMouseButtonPressed(MouseButton code);
		static void PressMouseButton(MouseButton code);
		static void ReleaseMouseButton(MouseButton code);

		static void Enable();
		static void Disable();
		static void Update();

		static void SetMousePosition(Vec2 position);
		static Vec2 GetMousePosition();
		static Vec2 GetPrevMousePosition();
		static Vec2 GetDeltaMousePosition();
		

	private:
		static std::unordered_map<KeyCode, bool> m_keys;
		static std::unordered_map<MouseButton, bool> m_mouseButtons;
		static Vec2 m_mousePosition;
		static Vec2 m_prevMousePosition;
	};
}