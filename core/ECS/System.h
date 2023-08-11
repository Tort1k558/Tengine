#pragma once

#include<memory>

#include"Core/Core.h"

namespace Tengine
{
    class TENGINE System {
    public:
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void destroy() = 0;

    protected:
        System() = default;
        ~System() = default;
    private:
        System(const System&) = delete;
        System& operator=(const System&) = delete;
    };
}