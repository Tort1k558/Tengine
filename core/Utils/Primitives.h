#pragma once


#include"Core/Math.h"
#include"Utils/Mesh.h"

namespace Tengine
{

    namespace Primitives
    {
        TENGINE std::shared_ptr<Mesh> CreateQuad();
        TENGINE std::shared_ptr<Mesh> CreateSphere(int sectors, int stacks);
        TENGINE std::shared_ptr<Mesh> CreateCube();
    }
}