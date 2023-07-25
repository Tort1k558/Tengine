#include "Primitives.h"

std::shared_ptr<Mesh> Primitives::CreateQuad()
{
    std::vector<Vertex> vertices;
    vertices.push_back({ Vec3(-1.0f, -1.0f, 0.0f) ,Vec3(0.0f,0.0f,1.0f),Vec2(0.0f, 0.0f) });
    vertices.push_back({ Vec3(1.0f,  1.0f, 0.0f) ,Vec3(0.0f,0.0f,1.0f),Vec2(1.0f, 1.0f) });
    vertices.push_back({ Vec3(-1.0f,  1.0f, 0.0f) ,Vec3(0.0f,0.0f,1.0f),Vec2(0.0f, 1.0f) });
    vertices.push_back({ Vec3(1.0f, -1.0f, 0.0f) ,Vec3(0.0f,0.0f,1.0f),Vec2(1.0f, 0.0f) });
    std::vector<unsigned int> indices = { 0,1,2,
                                          0,1,3 };
    return Component::Create<Mesh>(vertices, indices);
}

std::shared_ptr<Mesh> Primitives::CreateSphere(int sectors, int stacks)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float radius = 1.0f;
    const float sectorStep = 2.0f * PI / static_cast<float>(sectors);
    const float stackStep = PI / static_cast<float>(stacks);

    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = PI / 2.0f - static_cast<float>(i) * stackStep;
        float xy = radius * std::cos(stackAngle);
        float z = radius * std::sin(stackAngle);

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = static_cast<float>(j) * sectorStep;
            float x = xy * std::cos(sectorAngle);
            float y = xy * std::sin(sectorAngle);
            float u = static_cast<float>(j) / static_cast<float>(sectors);
            float v = static_cast<float>(i) / static_cast<float>(stacks);

            Vertex vertex;
            vertex.position = { x,y,z };
            vertex.texCoords = { u,v };
            vertex.normal = Normalize(Vec3(x, y, z));

            vertices.push_back(vertex);
        }
    }

    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    return Component::Create<Mesh>(vertices, indices);
}

std::shared_ptr<Mesh> Primitives::CreateCube()
{
    std::vector<Vertex> vertices;
    //                          Front
    vertices.push_back({ Vec3(-1.0f, -1.0f,  1.0f) ,Vec3(0.0f,0.0f,1.0f),Vec2(0.0f, 0.0f) });
    vertices.push_back({ Vec3(1.0f,  1.0f,  1.0f) ,Vec3(0.0f,0.0f,1.0f),Vec2(1.0f, 1.0f) });
    vertices.push_back({ Vec3(-1.0f,  1.0f,  1.0f) ,Vec3(0.0f,0.0f,1.0f),Vec2(0.0f, 1.0f) });
    vertices.push_back({ Vec3(1.0f, -1.0f,  1.0f) ,Vec3(0.0f,0.0f,1.0f),Vec2(1.0f, 0.0f) });
    //                          Back
    vertices.push_back({ Vec3(-1.0f, -1.0f, -1.0f) ,Vec3(0.0f,0.0f,-1.0f),Vec2(1.0f, 0.0f) });
    vertices.push_back({ Vec3(1.0f,  1.0f, -1.0f) ,Vec3(0.0f,0.0f,-1.0f),Vec2(0.0f, 1.0f) });
    vertices.push_back({ Vec3(-1.0f,  1.0f, -1.0f) ,Vec3(0.0f,0.0f,-1.0f),Vec2(1.0f, 1.0f) });
    vertices.push_back({ Vec3(1.0f, -1.0f, -1.0f) ,Vec3(0.0f,0.0f,-1.0f),Vec2(0.0f, 0.0f) });
    //                          Left
    vertices.push_back({ Vec3(-1.0f, -1.0f, -1.0f) ,Vec3(-1.0f,0.0f,0.0f),Vec2(0.0f, 0.0f) });
    vertices.push_back({ Vec3(-1.0f,  1.0f,  1.0f) ,Vec3(-1.0f,0.0f,0.0f),Vec2(1.0f, 1.0f) });
    vertices.push_back({ Vec3(-1.0f,  1.0f, -1.0f) ,Vec3(-1.0f,0.0f,0.0f),Vec2(0.0f, 1.0f) });
    vertices.push_back({ Vec3(-1.0f, -1.0f,  1.0f) ,Vec3(-1.0f,0.0f,0.0f),Vec2(1.0f, 0.0f) });
    //                          Right
    vertices.push_back({ Vec3(1.0f, -1.0f, -1.0f) ,Vec3(1.0f,0.0f,0.0f),Vec2(1.0f, 0.0f) });
    vertices.push_back({ Vec3(1.0f,  1.0f,  1.0f) ,Vec3(1.0f,0.0f,0.0f),Vec2(0.0f, 1.0f) });
    vertices.push_back({ Vec3(1.0f,  1.0f, -1.0f) ,Vec3(1.0f,0.0f,0.0f),Vec2(1.0f, 1.0f) });
    vertices.push_back({ Vec3(1.0f, -1.0f,  1.0f) ,Vec3(1.0f,0.0f,0.0f),Vec2(0.0f, 0.0f) });
    //                          Top
    vertices.push_back({ Vec3(-1.0f,  1.0f, -1.0f) ,Vec3(0.0f,1.0f,0.0f),Vec2(0.0f, 1.0f) });
    vertices.push_back({ Vec3(1.0f,  1.0f,  1.0f) ,Vec3(0.0f,1.0f,0.0f),Vec2(1.0f, 0.0f) });
    vertices.push_back({ Vec3(1.0f,  1.0f, -1.0f) ,Vec3(0.0f,1.0f,0.0f),Vec2(1.0f, 1.0f) });
    vertices.push_back({ Vec3(-1.0f,  1.0f,  1.0f) ,Vec3(0.0f,1.0f,0.0f),Vec2(0.0f, 0.0f) });
    //                          Bottom
    vertices.push_back({ Vec3(-1.0f, -1.0f, -1.0f) ,Vec3(0.0f,-1.0f,0.0f),Vec2(0.0f, 1.0f) });
    vertices.push_back({ Vec3(1.0f, -1.0f,  1.0f) ,Vec3(0.0f,-1.0f,0.0f),Vec2(1.0f, 0.0f) });
    vertices.push_back({ Vec3(1.0f, -1.0f, -1.0f) ,Vec3(0.0f,-1.0f,0.0f),Vec2(1.0f, 1.0f) });
    vertices.push_back({ Vec3(-1.0f, -1.0f,  1.0f) ,Vec3(0.0f,-1.0f,0.0f),Vec2(0.0f, 0.0f) });
    // Front
    std::vector<unsigned int> indices = { 0, 1, 2,  0, 1, 3,
        // Back
      4, 5, 6,  4, 5, 7,
        // Left
      8, 9, 10, 8, 9, 11,
        // Right
      12,13,14, 12,13,15,
        // Top
      16,17,18, 16,17,19,
        // Bottom
      20,21,22, 20,21,23 };

    return Component::Create<Mesh>(vertices, indices);
}