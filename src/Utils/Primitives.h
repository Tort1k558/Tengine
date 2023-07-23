#pragma once


#include"Core/Math.h"
#include"Components/Mesh.h"
namespace Primitives
{
    std::shared_ptr<Mesh> CreateQuad();
    std::shared_ptr<Mesh> CreateSphere(int sectors, int stacks);
    std::shared_ptr<Mesh> CreateCube();
}