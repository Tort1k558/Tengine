#pragma once

namespace Tengine
{
	namespace GLSL
	{
		const char* defaultShaderVertex = R"(#version 460

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUv;

out vec2 uv;
out vec3 normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	normal = normalize(mat3(transpose(inverse(u_model))) * vertexNormal);
    uv = vertexUv;
    gl_Position = u_projection * u_view * u_model * vec4(vertexPos,1.0);
})";
		const char* defaultShaderFragment = R"(#version 460 

layout (binding = 0)uniform sampler2D ourTexture;

in vec2 uv;
in vec3 normal;
out vec4 fragColor;

void main()
{
	vec3 lightDirection = normalize(vec3(0.0,-1.0,0.0));
	//fragColor = vec4(uv.r, uv.g, 0.0,1.0);
	//fragColor = vec4(normal,1.0);
    fragColor = texture(ourTexture,uv);
})";

	}
}