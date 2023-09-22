#version 460

layout(location = 0) in vec3 vertexPos;
layout(location = 2) in vec2 vertexUv;

out vec2 uv;

void main()
{
    uv = vertexUv;
    gl_Position = vec4(vertexPos,1.0);
}