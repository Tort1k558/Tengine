#pragma once

#include<memory>

namespace Tengine
{
    class System {
    public:
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void destroy() = 0;

        template<typename T>
        static std::shared_ptr<T> GetInstance() {
            static std::shared_ptr<T> instance;
            if (!instance)
            {
                instance = std::make_shared<T>();
            }
            return instance;
        }

    protected:
        System() = default;
        ~System() = default;

    private:
        System(const System&) = delete;
        System& operator=(const System&) = delete;
    };
}