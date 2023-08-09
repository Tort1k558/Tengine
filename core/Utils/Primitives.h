#pragma once


#include"Core/Math.h"
#include"ECS/Components/Mesh.h"

namespace Tengine
{

    namespace Primitives
    {
        std::shared_ptr<Mesh> CreateQuad();
        std::shared_ptr<Mesh> CreateSphere(int sectors, int stacks);
        std::shared_ptr<Mesh> CreateCube();
    }
}