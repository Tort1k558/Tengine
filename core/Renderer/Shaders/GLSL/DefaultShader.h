#pragma once

namespace Tengine
{
	namespace GLSL
	{
		const char* defaultShaderVertex = R"(#version 460

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 uv;
out vec3 normal;
out vec3 fragPos;

void main()
{
	fragPos = vec3(u_model * vec4(vertexPos,1.0));
	normal = normalize(mat3(transpose(inverse(u_model))) * vertexNormal);
    uv = vertexUv;
    gl_Position = u_projection * u_view * u_model * vec4(vertexPos,1.0);
})";
		const char* defaultShaderFragment = R"(#version 460 

layout (binding = 0) uniform sampler2D albedoMap;
layout (binding = 1) uniform sampler2D normalMap;
layout (binding = 2) uniform sampler2D specularMap;
layout (binding = 3) uniform sampler2D rougnessMap;

uniform vec3 u_viewPos;

in vec2 uv;
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

void main()
{
	vec3 albedo = texture(albedoMap, uv).rgb;
    vec3 normalFromTexture = texture(normalMap, uv).rgb;
	normalFromTexture = normalize(normalFromTexture * 2.0 - 1.0);


    fragColor = vec4(albedo,1.0);
})";
	}

	namespace ESSL
	{
		const char* defaultShaderVertex = R"(#version 300 es
precision mediump float;

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 uv;
out vec3 normal;

void main()
{
	normal = normalize(mat3(transpose(inverse(u_model))) * vertexNormal);
    uv = vertexUv;
    gl_Position = u_projection * u_view * u_model * vec4(vertexPos,1.0);
})";
		const char* defaultShaderFragment = R"(#version 300 es
precision mediump float;
uniform sampler2D ourTexture;

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