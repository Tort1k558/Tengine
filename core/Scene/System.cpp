#include "System.h"

namespace Tengine
{
    bool System::isInitialized()
    {
        return m_isInitialized;
    }

    size_t System::getUpdatesPerSecond()
    {
        return m_updatesPerSecond;
    }

    void System::setUpdatesPerSecond(size_t count)
    {
        m_updatesPerSecond = count;
    }
}
