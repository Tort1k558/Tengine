#pragma once

#include<memory>

#include"Core/Core.h"

namespace Tengine
{
    class TENGINE System {
    public:
        System(const System&) = delete;
        System& operator=(const System&) = delete;
        virtual ~System() = default;

        virtual void init() = 0;
        virtual void update() = 0;
        virtual void destroy() = 0;
        
        bool isInitialized();
        size_t getUpdatesPerSecond();
        void setUpdatesPerSecond(size_t count);
    protected:
        System() = default;

        bool m_isInitialized = false;
        size_t m_updatesPerSecond = 0;

        friend class SystemManager;
    };
}