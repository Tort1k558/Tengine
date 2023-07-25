#pragma once

#include"Core/Application.h"


class Editor : public Application
{
public:
    Editor(unsigned int width, unsigned int height, const std::string& title);
    void create() final;

    void update() final;
};