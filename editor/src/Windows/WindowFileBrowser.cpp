#include "WindowFileBrowser.h"

#include<imgui/imgui.h>
#include<imgui/misc/cpp/imgui_stdlib.h>

#include<Scene/SceneManager.h>
#include<Utils/Material.h>
#include<Utils/Primitives.h>
#include<Components/Model.h>
#include<Utils/FileManager.h>

#include"WindowMonitor.h"
#include"UIRender.h"
#include"ProjectManager.h"
#include"Editor.h"

namespace TengineEditor
{
    std::filesystem::path WindowFileBrowser::m_relativePath = "Assets";

    void WindowFileBrowser::Render()
    {
        ImGui::Begin("File Browser", nullptr, ImGuiWindowFlags_NoScrollbar);

        static int selectedFileIndex = -1;
        float windowWidth = ImGui::GetContentRegionAvail().x;
        if (ImGui::BeginTable("TreeOfFiles", 2, ImGuiTableFlags_Resizable))
        {
            ImGui::TableSetupColumn("Path:");
            ImGui::TableSetupColumn(GetRelativePath().string().c_str(), ImGuiTableColumnFlags_WidthFixed, windowWidth * 0.3f);
            ImGui::TableHeadersRow();

            //Tree
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            if (ImGui::BeginListBox("##empty", ImVec2(-FLT_MIN, -FLT_MIN)))
            {

                if (ImGui::TreeNodeEx("Assets", ImGuiTreeNodeFlags_OpenOnDoubleClick))
                {
                    if (ImGui::IsItemClicked())
                    {
                        SetRelativePath("Assets");
                    }
                    std::function<void(std::filesystem::path)> renderFileTree = [&renderFileTree](std::filesystem::path path)
                        {
                            if (std::filesystem::is_directory(path.string()))
                            {
                                bool treeIsOpened = ImGui::TreeNodeEx(path.filename().string().c_str(), ImGuiTreeNodeFlags_OpenOnDoubleClick);
                                if (ImGui::IsItemClicked())
                                {
                                    SetRelativePath(path);
                                }
                                if (treeIsOpened)
                                {
                                    std::vector<std::filesystem::path> directoryFiles = FileManager::GetFilesFromCurrentDirectory(path);
                                    for (const auto& childPath : directoryFiles)
                                    {
                                        renderFileTree(childPath);
                                    }
                                    ImGui::TreePop();
                                }
                            }
                            else
                            {
                                ImGui::Button(path.filename().string().c_str());
                            }
                        };
                    std::vector<std::filesystem::path> pathToProjectFiles = FileManager::GetFilesFromCurrentDirectory(Editor::GetPathToAssets());
                    for (int i = 0; i < pathToProjectFiles.size(); i++)
                    {
                        renderFileTree(pathToProjectFiles[i]);
                    }
                    ImGui::TreePop();
                }
                ImGui::EndListBox();
            }

            //Files
            ImGui::TableNextColumn();

            if (ImGui::BeginListBox("##empty2", ImVec2(-FLT_MIN, -FLT_MIN)))
            {
                if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
                {
                    ImGui::OpenPopup("FileBrowserContextMenu");
                }

                bool openPrimitiveMenu = false;
                if (ImGui::BeginPopup("FileBrowserContextMenu"))
                {
                    if (ImGui::MenuItem("Create File"))
                    {
                        FileManager::NewFile(GetRelativePath().string() + "/NewFile");
                    }
                    if (ImGui::MenuItem("Create Folder"))
                    {
                        FileManager::NewFolder(GetRelativePath().string() + "/NewFolder");
                    }
                    if (ImGui::MenuItem("Create Model"))
                    {
                        std::shared_ptr<Model> model = Component::Create<Model>();
                        model->setPath(FileManager::GetUniqueFilePath(GetRelativePath().string() + "/Model.model"));
                        AssetManager::SaveModel(model.get());
                    }
                    if (ImGui::MenuItem("Create Material"))
                    {
                        std::shared_ptr<Material> material = std::make_shared<Material>();
                        material->setPath(FileManager::GetUniqueFilePath(GetRelativePath().string() + "/Material.material"));
                        AssetManager::SaveMaterial(material.get());
                    }
                    if (ImGui::MenuItem("Create Primitive"))
                    {
                        openPrimitiveMenu = true;
                    }
                    ImGui::EndPopup();
                }

                //Creating Primitive
                if (openPrimitiveMenu)
                {
                    ImGui::OpenPopup("PrimitiveMenu");
                }

                if (ImGui::BeginPopupModal("PrimitiveMenu"))
                {
                    std::vector<std::string> primitiveNames = { "Quad","Sphere","Cube" };
                    static int currentElement = 0;
                    UIRender::DrawCombo("Primitive", primitiveNames, currentElement);
                    static int sectors = 1;
                    static int stacks = 1;
                    if (primitiveNames[currentElement] == "Sphere")
                    {
                        ImGui::InputInt("Sectors", &sectors);
                        ImGui::InputInt("Stacks", &stacks);
                    }
                    if (ImGui::Button("Create"))
                    {
                        if (primitiveNames[currentElement] == "Quad")
                        {
                            std::shared_ptr<Model> model = Component::Create<Model>();
                            model->setPath(FileManager::GetUniqueFilePath(GetRelativePath().string() + "/Quad.model"));
                            model->setMesh(Primitives::CreateQuad());
                        }
                        else  if (primitiveNames[currentElement] == "Sphere")
                        {
                            std::shared_ptr<Model> model = Component::Create<Model>();
                            model->setPath(FileManager::GetUniqueFilePath(GetRelativePath().string() + "/Sphere.model"));
                            model->setMesh(Primitives::CreateSphere(sectors, stacks));
                        }
                        else  if (primitiveNames[currentElement] == "Cube")
                        {
                            std::shared_ptr<Model> model = Component::Create<Model>();
                            model->setPath(FileManager::GetUniqueFilePath(GetRelativePath().string() + "/Cube.model"));
                            model->setMesh(Primitives::CreateCube());
                        }
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                float tableFilesWidth = ImGui::GetContentRegionAvail().x;
                float cellSize = 64.0f;
                float cellSizeWithPadding = cellSize + 16.0f;
                int columnCount = static_cast<int>(tableFilesWidth / cellSizeWithPadding);
                if (columnCount < 1)
                {
                    columnCount = 1;
                }
                if (ImGui::BeginTable("Files", columnCount))
                {
                    std::vector<std::filesystem::path> pathToCurrentProjectFiles = FileManager::GetFilesFromCurrentDirectory(GetRelativePath());
                    ImGui::TableNextColumn();
                    for (int i = 0; i < pathToCurrentProjectFiles.size(); i++)
                    {
                        ImGui::PushID(pathToCurrentProjectFiles[i].string().c_str());

                        if (std::filesystem::is_directory(pathToCurrentProjectFiles[i].string()))
                        {
                            ImGui::ImageButton((void*)(AssetManager::LoadTexture(Editor::GetPathToEditor().string() + "/data/folder.png")->getId()), ImVec2(cellSize, cellSize), { 0,1 }, { 1,0 });

                            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
                            {
                                SetRelativePath(pathToCurrentProjectFiles[i].string());
                            }
                        }
                        else
                        {
                            ImGui::ImageButton((void*)(AssetManager::LoadTexture(Editor::GetPathToEditor().string() + "/data/file.png")->getId()), ImVec2(cellSize, cellSize), { 0,1 }, { 1,0 });

                            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
                            {
                                WindowMonitor::SetPathMonitoringFile(pathToCurrentProjectFiles[i]);
                            }
                            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
                            {
                                if (pathToCurrentProjectFiles[i].extension() == ".scene")
                                {
                                    SceneManager::LoadByPath(pathToCurrentProjectFiles[i]);
                                }
                                else
                                {
                                    std::string command = "start " + pathToCurrentProjectFiles[i].string();
                                    std::system(command.c_str());
                                }
                            }
                        }

                        if (ImGui::BeginDragDropSource())
                        {
                            ImGui::SetDragDropPayload("path", pathToCurrentProjectFiles[i].string().c_str(),
                                pathToCurrentProjectFiles[i].string().size() * sizeof(std::string::value_type));
                            ImGui::Text(pathToCurrentProjectFiles[i].string().c_str());
                            ImGui::EndDragDropSource();
                        }
                        ImGui::PopID();

                        if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
                        {
                            selectedFileIndex = i;
                            ImGui::OpenPopup("FileContextMenu");
                        }


                        //Rename File
                        static bool isEditName = false;
                        static std::string newName;
                        if (ImGui::IsKeyPressed(ImGuiKey_F2) && ImGui::IsItemHovered())
                        {
                            newName = pathToCurrentProjectFiles[i].filename().string();
                            selectedFileIndex = i;
                            isEditName = true;
                        }
                        if (isEditName && selectedFileIndex == i)
                        {
                            if (ImGui::InputText("", &newName, ImGuiInputTextFlags_EnterReturnsTrue))
                            {
                                isEditName = false;
                                FileManager::RenameFile(pathToCurrentProjectFiles[i].string(), newName);
                            }
                        }
                        else
                        {
                            ImGui::TextWrapped(pathToCurrentProjectFiles[i].filename().string().c_str());
                        }

                        ImGui::TableNextColumn();
                    }

                    if (ImGui::BeginPopup("FileContextMenu"))
                    {
                        if (ImGui::MenuItem("Remove File"))
                        {
                            if (selectedFileIndex >= 0 && selectedFileIndex < pathToCurrentProjectFiles.size())
                            {
                                FileManager::RemoveFile(pathToCurrentProjectFiles[selectedFileIndex].string());
                                selectedFileIndex = -1;
                            }
                        }
                        ImGui::EndPopup();
                    }

                    ImGui::EndTable();
                }
                ImGui::EndListBox();
            }
            ImGui::EndTable();
        }

        ImGui::End();
    }
    void WindowFileBrowser::SetRelativePath(std::filesystem::path path)
    {
        m_relativePath = path;
    }

    std::filesystem::path WindowFileBrowser::GetCurrentPath()
    {
        if (ProjectManager::GetInstance())
        {
            return m_relativePath.string();
        }
        return "";
    }

    std::filesystem::path WindowFileBrowser::GetRelativePath()
    {
        return m_relativePath;
    }
}
