#include "Input.h"

#include"Core/Logger.h"

std::unordered_map<KeyCode, bool> Input::m_keys;
std::unordered_map<MouseCode, bool> Input::m_mouseButtons;

bool Input::IsKeyPressed(KeyCode code)
{
    return m_keys[code];
}

void Input::PressKey(KeyCode code)
{
    Logger::Debug("EVENT::The mouse button is pressed with the code {0}", static_cast<int>(code));
    m_keys[code] = true;
}

void Input::ReleaseKey(KeyCode code)
{
    Logger::Debug("EVENT::The mouse button is released with the code {0}", static_cast<int>(code));;
    m_keys[code] = false;
}

bool Input::isMouseButtonPressed(MouseCode code)
{
    return m_mouseButtons[code];
}

void Input::PressMouseButton(MouseCode code)
{
    Logger::Debug("EVENT::The mouse button is pressed with the code {0}", static_cast<int>(code));
    m_mouseButtons[code] = true;
}

void Input::ReleaseMouseButton(MouseCode code)
{
    Logger::Debug("EVENT::The mouse button is released with the code {0}", static_cast<int>(code));
    m_mouseButtons[code] = false;
}
