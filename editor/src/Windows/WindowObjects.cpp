#include "WindowObjects.h"

#include<imgui/imgui.h>
#include<imgui/misc/cpp/imgui_stdlib.h>
#include<Scene/SceneManager.h>

#include"WindowMonitor.h"

namespace TengineEditor
{
    std::shared_ptr<Object> WindowObjects::m_selectedObject;

    void WindowObjects::Render()
    {
        ImGui::Begin("Objects", nullptr);
        if (SceneManager::GetCurrentScene())
        {
            std::vector<std::shared_ptr<Object>> objects = SceneManager::GetCurrentScene()->getAllObjects();
            std::vector<std::string> objectNames;
            for (const auto& object : objects)
            {
                objectNames.push_back(object->getName());
            }
            std::sort(objectNames.begin(), objectNames.end(), std::less<std::string>());

            if (ImGui::BeginListBox("##empty"))
            {
                for (const auto& object : objects)
                {
                    static bool isEditName = false;
                    static std::string newName;
                    if (isEditName && GetSelectedObject()->getId() == object->getId())
                    {
                        if (ImGui::InputText("##InputName", &newName, ImGuiInputTextFlags_EnterReturnsTrue))
                        {
                            isEditName = false;
                            object->setName(newName);
                        }
                    }
                    else
                    {
                        bool isSelected = false;
                        if (GetSelectedObject())
                        {
                            isSelected = GetSelectedObject()->getId() == object->getId();
                        }
                        if (ImGui::Selectable(object->getName().c_str(), isSelected))
                        {
                            SetSelectedObject(SceneManager::GetCurrentScene()->getObjectByName(object->getName()));
                            isEditName = false;
                            WindowMonitor::SetMonitoringObject(GetSelectedObject());
                        }
                        if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
                        {
                            SetSelectedObject(SceneManager::GetCurrentScene()->getObjectByName(object->getName()));
                            isEditName = false;
                            WindowMonitor::SetMonitoringObject(GetSelectedObject());
                            ImGui::OpenPopup("ObjectContextMenu");
                        }
                        if (ImGui::IsKeyPressed(ImGuiKey_F2) && ImGui::IsItemHovered())
                        {
                            SetSelectedObject(SceneManager::GetCurrentScene()->getObjectByName(object->getName()));
                            newName = GetSelectedObject()->getName();
                            isEditName = true;
                        }
                    }
                }

                if (ImGui::BeginPopup("ObjectContextMenu"))
                {
                    if (ImGui::MenuItem("Delete object"))
                    {
                        SceneManager::GetCurrentScene()->removeObjectByName(GetSelectedObject()->getName());
                        WindowMonitor::SetMonitoringObject(nullptr);
                        SetSelectedObject(nullptr);
                    }
                    ImGui::EndPopup();
                }

                ImGui::EndListBox();
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y - 25.0f);

            if (ImGui::Button("Create object"))
            {
                Object::Create();
            }
        }
        ImGui::End();
    }

    void WindowObjects::SetSelectedObject(std::shared_ptr<Object> object)
    {
        m_selectedObject = object;
    }

    std::shared_ptr<Object> WindowObjects::GetSelectedObject()
    {
        return m_selectedObject;
    }

}
