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

layout (binding = 0) uniform sampler2D ourTexture;

in vec2 uv;
out vec4 fragColor;

void main()
{
    fragColor = texture(ourTexture,uv);
})";
		const char* PostProcessingShaderFragment = R"(#version 460 

layout (binding = 0) uniform sampler2D ourTexture;

uniform float u_gamma;

in vec2 uv;
out vec4 fragColor;

void main()
{
    vec3 result = texture(ourTexture,uv);

	result = pow(result,vec3(1.0 / u_gamma));
	
	fragColor = vec4(result,1.0);
})";
	}
	namespace ESSL
	{
		const char* framebufferShaderVertex = R"(#version 300 es
precision mediump float;

layout(location = 0) in vec3 vertexPos;
layout(location = 2) in vec2 vertexUv;

out vec2 uv;

void main()
{
    uv = vertexUv;
    gl_Position = vec4(vertexPos,1.0);
})";
		const char* framebufferShaderFragment = R"(#version 300 es
precision mediump float;

uniform sampler2D ourTexture;

in vec2 uv;
out vec4 fragColor;

void main()
{
    fragColor = texture(ourTexture,uv);
})";
	}
}