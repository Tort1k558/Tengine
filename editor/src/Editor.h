#pragma once

#include<filesystem>

#include<Core/Application.h>

namespace TengineEditor
{
    using namespace Tengine;

    class Editor : public Application
    {
    public:
        Editor(unsigned int width, unsigned int height, const std::string& title);
        void create() final;
        void update() final;
        void close() final;

        static std::filesystem::path GetPathToEditor();
        static std::filesystem::path Editor::GetPathToAssets();
    private:
        static std::filesystem::path m_pathToEditor;
    };
}