#include "UIRender.h"

#include<imgui/imgui.h>
#include<imgui/misc/cpp/imgui_stdlib.h>

namespace TengineEditor
{
    bool UIRender::DrawFloat(std::string_view name, float* data, float speed, float min, float max)
    {
        if (ImGui::DragFloat(name.data(), data, speed, min, max))
        {
            return true;
        }
        return false;
    }

    bool UIRender::DrawVec2(std::string_view name, float* data, float speed, float min, float max)
    {
        if (ImGui::DragFloat2(name.data(), data, speed, min, max))
        {
            return true;
        }
        return false;
    }

    bool UIRender::DrawVec3(std::string_view name, float* data, float speed, float min, float max)
    {
        if (ImGui::DragFloat3(name.data(), data, speed, min, max))
        {
            return true;
        }
        return false;
    }

    bool UIRender::DrawVec4(std::string_view name, float* data, float speed, float min, float max)
    {
        if (ImGui::DragFloat4(name.data(), data, speed, min, max))
        {
            return true;
        }
        return false;
    }

    void UIRender::DrawImage(void* id, Vec2 size)
    {
        ImGui::Image(id, { size.x, size.y });
    }

    bool UIRender::DrawCombo(std::string_view name, std::vector<std::string> elements,int& currentElement)
    {
        bool result = false;
        if (ImGui::BeginCombo(name.data(), elements[0].c_str()))
        {
            for (int i = 0; i < elements.size(); ++i)
            {
                const bool isSelected = (currentElement == i);
                if (ImGui::Selectable(elements[i].c_str(), isSelected))
                {
                    currentElement = i;
                    result = true;
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        return result;
    }

    bool UIRender::DrawButton(std::string_view name)
    {
        if (ImGui::Button(name.data()))
        {
            return true;
        }
        return false;
    }

    bool UIRender::DrawCheckbox(std::string_view name, bool* data)
    {
        if (ImGui::Checkbox(name.data(), data))
        {
            return true;
        }
        return false;
    }

    bool UIRender::DrawCollapsingHeader(std::string_view name)
    {
        if (ImGui::CollapsingHeader(name.data()))
        {
            return true;
        }
        return false;
    }

    bool UIRender::DrawFile(std::string_view name, std::string& pathToFile)
    {
        bool result = false;
        ImGui::Text(name.data());
        ImGui::SameLine();
        ImGui::InputText(("##" + std::string(name)).c_str(), &pathToFile, ImGuiInputTextFlags_EnterReturnsTrue);

        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("path")) {
                std::string path(static_cast<const char*>(payload->Data), payload->DataSize);
                if (path != "")
                {
                    pathToFile = path;
                    result = true;
                }
            }
            ImGui::EndDragDropTarget();
        }
        return result;
    }
}
