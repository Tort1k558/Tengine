#include "WindowFileBrowser.h"

#include<imgui/imgui.h>
#include<imgui/misc/cpp/imgui_stdlib.h>

#include<Scene/SceneManager.h>

#include"FileManager.h"
#include"WindowMonitor.h"

namespace TengineEditor
{

    void WindowFileBrowser::Render()
    {
        ImGui::Begin("File Browser", nullptr, ImGuiWindowFlags_NoScrollbar);

        static int selectedFileIndex = -1;
        float windowWidth = ImGui::GetContentRegionAvail().x;
        if (ImGui::BeginTable("TreeOfFiles", 2, ImGuiTableFlags_Resizable))
        {
            ImGui::TableSetupColumn("Path:");
            ImGui::TableSetupColumn(FileManager::GetRelativePath().string().c_str(), ImGuiTableColumnFlags_WidthFixed, windowWidth * 0.3f);
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
                        FileManager::SetRelativePath("Assets");
                    }
                    std::function<void(std::filesystem::path)> renderFileTree = [&renderFileTree](std::filesystem::path path)
                        {
                            if (std::filesystem::is_directory(path.string()))
                            {
                                bool treeIsOpened = ImGui::TreeNodeEx(path.filename().string().c_str(), ImGuiTreeNodeFlags_OpenOnDoubleClick);
                                if (ImGui::IsItemClicked())
                                {
                                    FileManager::SetRelativePath(path);
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
                    std::vector<std::filesystem::path> pathToProjectFiles = FileManager::GetAllProjectFiles();
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

                if (ImGui::BeginPopup("FileBrowserContextMenu"))
                {
                    if (ImGui::MenuItem("Create File"))
                    {
                        FileManager::NewFile(FileManager::GetRelativePath().string() + "/NewFile");
                    }
                    if (ImGui::MenuItem("Create Folder"))
                    {
                        FileManager::NewFolder(FileManager::GetRelativePath().string() + "/NewFolder");
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
                    std::vector<std::filesystem::path> pathToCurrentProjectFiles = FileManager::GetFilesFromCurrentDirectory(FileManager::GetRelativePath());
                    ImGui::TableNextColumn();
                    for (int i = 0; i < pathToCurrentProjectFiles.size(); i++)
                    {
                        ImGui::PushID(pathToCurrentProjectFiles[i].string().c_str());

                        if (std::filesystem::is_directory(pathToCurrentProjectFiles[i].string()))
                        {
                            ImGui::ImageButton((void*)(AssetManager::LoadTexture(FileManager::GetPathToEditor().string() + "/data/folder.png")->getId()), ImVec2(cellSize, cellSize), { 0,1 }, { 1,0 });

                            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
                            {
                                FileManager::SetRelativePath(pathToCurrentProjectFiles[i].string());
                            }
                        }
                        else
                        {
                            ImGui::ImageButton((void*)(AssetManager::LoadTexture(FileManager::GetPathToEditor().string() + "/data/file.png")->getId()), ImVec2(cellSize, cellSize), { 0,1 }, { 1,0 });

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

}
