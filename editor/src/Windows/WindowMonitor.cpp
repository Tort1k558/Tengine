#include "WindowMonitor.h"

#include<imgui/imgui.h>
#include<imgui/misc/cpp/imgui_stdlib.h>

#include<Systems/ScriptSystem.h>

#include<Components/Camera.h>
#include<Components/Model.h>
#include"Components/Light.h"

#include"UIRender.h"

namespace TengineEditor
{
    WindowMonitor::MonitoringType WindowMonitor::m_monitoringType = MonitoringType::None;
    std::unordered_map<std::string, std::function<void(std::filesystem::path)>> WindowMonitor::m_formatHandlers;
    std::shared_ptr<Object> WindowMonitor::m_monitoringObject;
    std::filesystem::path WindowMonitor::m_pathToMonitoringFile;

	void WindowMonitor::Render()
	{
        ImGui::Begin("Monitor", nullptr);
        switch (m_monitoringType)
        {
        case MonitoringType::None:
            break;
        case MonitoringType::Object:
        {
            if (m_monitoringObject)
            {
                bool contextMenuOpened = false;
                static std::shared_ptr<Component> selectedComponent;
                std::shared_ptr<Object> object = m_monitoringObject;
                std::vector<std::shared_ptr<Component>> components = object->getComponents();
                for (const auto& component : components)
                {
                    ComponentInfo info = component->getInfo();
                    if (!info.getComponentName().empty())
                    {
                        if (UIRender::DrawCollapsingHeader(info.getComponentName()))
                        {
                            for (const auto& element : info.getElements())
                            {
                                ShowField(element);
                            }
                        }
                        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
                        {
                            contextMenuOpened = true;
                            selectedComponent = component;
                        }
                    }
                }
                if (contextMenuOpened)
                {
                    ImGui::OpenPopup("Component Menu");
                    contextMenuOpened = false;
                }

                if (ImGui::BeginPopup("Component Menu"))
                {
                    if (ImGui::MenuItem("Delete Component"))
                    {
                        if (selectedComponent)
                        {
                            object->removeComponent(selectedComponent);
                            selectedComponent.reset();
                        }
                    }
                    ImGui::EndPopup();
                }

                if (ImGui::Button("Add component"))
                {
                    ImGui::OpenPopup("Select Component");
                }
                if (ImGui::BeginPopup("Select Component"))
                {
                    static int selectedItem = 0;
                    std::vector<std::string> items = { "Model", "Camera","Direction Light","Point Light","Spot Light" };
                    for (const auto& scriptName : ScriptSystem::GetInstance()->getScriptNames())
                    {
                        items.push_back(scriptName);
                    }
                    UIRender::DrawCombo("##Components", items, selectedItem);
                    if (ImGui::Button("Add"))
                    {
                        if (selectedItem == 0)
                        {
                            object->addComponent(Component::Create<Model>());
                        }
                        if (selectedItem == 1)
                        {
                            object->addComponent(Component::Create<Camera>());
                        }
                        if (selectedItem == 2)
                        {
                            object->addComponent(Component::Create<DirectionLight>());
                        }
                        if (selectedItem == 3)
                        {
                            object->addComponent(Component::Create<PointLight>());
                        }
                        if (selectedItem == 4)
                        {
                            object->addComponent(Component::Create<SpotLight>());
                        }
                        if (selectedItem >= 5)
                        {
                            ScriptSystem::GetInstance()->addScript(object, items[selectedItem]);
                        }
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
            }
            break;
        }
        case MonitoringType::File:
            if (!m_pathToMonitoringFile.empty())
            {
                ImGui::Text(m_pathToMonitoringFile.string().c_str());
                ImGui::Separator();
                if (m_formatHandlers.find(m_pathToMonitoringFile.extension().string()) != m_formatHandlers.end())
                {
                    m_formatHandlers[m_pathToMonitoringFile.extension().string()](m_pathToMonitoringFile);
                }
            }
            break;
        default:
            break;
        }
        ImGui::End();
    }

    void WindowMonitor::SetMonitoringObject(std::shared_ptr<Object> object)
    {
        m_monitoringObject = object;
        m_monitoringType = MonitoringType::Object;
        m_pathToMonitoringFile = "";
    }

    void WindowMonitor::SetPathMonitoringFile(std::filesystem::path pathToFile)
    {
        m_pathToMonitoringFile = pathToFile;
        m_monitoringType = MonitoringType::File;
        m_monitoringObject = nullptr;
    }

    void WindowMonitor::AddFormatHandler(std::string_view format, std::function<void(std::filesystem::path pathToFile)> func)
    {
        m_formatHandlers.emplace(format, func);
    }

    void WindowMonitor::ShowField(std::shared_ptr<FieldInfo> element)
    {
        switch (element->type)
        {
        case FieldType::None:
            break;
        case FieldType::Float:
        {
            std::shared_ptr<FieldFloat> slider = std::dynamic_pointer_cast<FieldFloat>(element);
            if (UIRender::DrawFloat(slider->name, static_cast<float*>(slider->data), 0.0f, slider->minValue, slider->maxValue))
            {
                if (slider->callback)
                {
                    slider->callback();
                }
            }
            break;
        }
        case FieldType::Vec2:
        {
            std::shared_ptr<FieldVec2> slider = std::dynamic_pointer_cast<FieldVec2>(element);
            if (UIRender::DrawVec2(slider->name, static_cast<float*>(&slider->data->x), 0.0f, slider->minValue, slider->maxValue))
            {
                if (slider->callback)
                {
                    slider->callback();
                }
            }
            break;
        }
        case FieldType::Vec3:
        {
            std::shared_ptr<FieldVec3> slider = std::dynamic_pointer_cast<FieldVec3>(element);
            if (UIRender::DrawVec3(slider->name, static_cast<float*>(&slider->data->x), 0.0f, slider->minValue, slider->maxValue))
            {
                if (slider->callback)
                {
                    slider->callback();
                }
            }
            break;
        }
        case FieldType::Vec4:
        {
            std::shared_ptr<FieldVec4> slider = std::dynamic_pointer_cast<FieldVec4>(element);
            if (UIRender::DrawVec4(slider->name, static_cast<float*>(&slider->data->x), 0.0f, slider->minValue, slider->maxValue))
            {
                if (slider->callback)
                {
                    slider->callback();
                }
            }
            break;
        }
        case FieldType::Enum:
        {
            std::shared_ptr<FieldEnum> combo = std::dynamic_pointer_cast<FieldEnum>(element);
            if (UIRender::DrawCombo(combo->name, combo->elements, *combo->currentElement))
            {
                combo->callback(*combo->currentElement);
            }
            break;
        }
        case FieldType::CollapsingHeader:
        {
            std::shared_ptr<FieldCollapsingHeader> header = std::dynamic_pointer_cast<FieldCollapsingHeader>(element);
            if (UIRender::DrawCollapsingHeader(header->name.c_str()))
            {
                for (size_t i = 0; i < header->elements.size(); i++)
                {
                    ShowField(header->elements[i]);
                }
            };
            break;
        }
        case FieldType::File:
        {
            std::shared_ptr<FieldFile> file = std::dynamic_pointer_cast<FieldFile>(element);
            std::string filePath = file->path.string();
            if (UIRender::DrawFile(file->name, filePath))
            {
                file->path = filePath;
                file->callback(filePath);
            }
            break;
        }
        case FieldType::Bool:
        {
            std::shared_ptr<FieldBool> field = std::dynamic_pointer_cast<FieldBool>(element);
            if (UIRender::DrawCheckbox(field->name.c_str(), field->data))
            {
                if (field->callback)
                {
                    field->callback();
                }
            }
        }
        default:
            break;
        }
    }

}
