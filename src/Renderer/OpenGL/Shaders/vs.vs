#version 460
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexColor;

out vec3 color;

uniform mat4 u_modelMatrix;

void main()
{
    color = vertexColor;
    gl_Position = u_modelMatrix * vec4(vertexPos,1.0);
}