#include "Input.h"

#include"Core/Logger.h"

namespace Tengine
{

    std::unordered_map<KeyCode, bool> Input::m_keys;
    std::unordered_map<MouseButton, bool> Input::m_mouseButtons;
    Vec2 Input::m_mousePosition;
    Vec2 Input::m_prevMousePosition;

    bool Input::IsKeyPressed(KeyCode code)
    {
        return m_keys[code];
    }

    void Input::PressKey(KeyCode code)
    {
        m_keys[code] = true;
    }

    void Input::ReleaseKey(KeyCode code)
    {
        m_keys[code] = false;
    }

    bool Input::IsMouseButtonPressed(MouseButton code)
    {
        return m_mouseButtons[code];
    }

    void Input::PressMouseButton(MouseButton code)
    {
        m_mouseButtons[code] = true;
    }

    void Input::ReleaseMouseButton(MouseButton code)
    {
        m_mouseButtons[code] = false;
    }

    void Input::Update()
    {
        m_prevMousePosition = m_mousePosition;
    }

    void Input::SetMousePosition(Vec2 position)
    {
        m_mousePosition = position;
    }

    Vec2 Input::GetMousePosition()
    {
        return m_mousePosition;
    }

    Vec2 Input::GetPrevMousePosition()
    {
        return m_prevMousePosition;
    }

    Vec2 Input::GetDeltaMousePosition()
    {
        return m_prevMousePosition - m_mousePosition;;
    }
}