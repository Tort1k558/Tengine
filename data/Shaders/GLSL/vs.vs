#version 460

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 vertexNormal;
layout(location = 2) in vec2 vertexUv;

out vec2 uv;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

void main()
{
    uv = vertexUv;
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(vertexPos,1.0);
}