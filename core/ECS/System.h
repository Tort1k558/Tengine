#pragma once

#include<memory>

#include"Core/Core.h"

namespace Tengine
{
    class TENGINE System {
        struct SystemInfo
        {
            bool isInitialized = false;
        };
    public:
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void destroy() = 0;

    protected:
        System() = default;
        virtual ~System() = default;
        SystemInfo m_info;
    private:
        System(const System&) = delete;
        System& operator=(const System&) = delete;
        friend class SystemManager;
    };
}