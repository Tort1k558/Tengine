#pragma once

#include<unordered_map>

#include"Core/Keys.h"

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

	static bool isMouseButtonPressed(MouseCode code);
	static void PressMouseButton(MouseCode code);
	static void ReleaseMouseButton(MouseCode code);
private:
	static std::unordered_map<KeyCode, bool> m_keys;
	static std::unordered_map<MouseCode, bool> m_mouseButtons;
};