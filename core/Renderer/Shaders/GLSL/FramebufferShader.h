#pragma once

namespace Tengine
{
	namespace GLSL
	{
		const char* framebufferShaderVertex = R"(#version 460

layout(location = 0) in vec3 vertexPos;
layout(location = 2) in vec2 vertexUv;

out vec2 uv;

void main()
{
    uv = vertexUv;
    gl_Position = vec4(vertexPos,1.0);
})";
		const char* framebufferShaderFragment = R"(#version 460 

layout (binding = 0)uniform sampler2D ourTexture;

in vec2 uv;
out vec4 fragColor;

void main()
{
    fragColor = texture(ourTexture,uv);
})";

	}
}