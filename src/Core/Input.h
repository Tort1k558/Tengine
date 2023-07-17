#pragma once

#include<unordered_map>

#include"Core/Keys.h"
#include"Core/Math.h"

class Input
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

	static bool IsMouseButtonPressed(MouseCode code);
	static void PressMouseButton(MouseCode code);
	static void ReleaseMouseButton(MouseCode code);

	static void SetMousePosition(Vec2 position);
	static Vec2 GetMousePosition() { return m_mousePosition; }
private:
	static std::unordered_map<KeyCode, bool> m_keys;
	static std::unordered_map<MouseCode, bool> m_mouseButtons;
	static Vec2 m_mousePosition;
};