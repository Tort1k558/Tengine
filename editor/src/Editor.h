#pragma once

#include"Core/Application.h"


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
    };
}