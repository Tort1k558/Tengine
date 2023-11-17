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
    uv = vertexUv;
	normal = normalize(mat3(transpose(inverse(u_model))) * vertexNormal);
	fragPos = vec3(u_model * vec4(vertexPos,1.0));
    gl_Position = u_projection * u_view * u_model * vec4(vertexPos,1.0);
})";
		const char* defaultShaderFragment = R"(#version 460 

struct SubMaterial
{
    sampler2D texture;
    vec3 color;
};

struct Material 
{
    SubMaterial albedo;
    SubMaterial normal;
    SubMaterial specular;
    SubMaterial roughness;
}; 

uniform Material u_material;

uniform vec3 u_viewPos;

in vec2 uv;
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

void main()
{
	vec3 albedo = texture(u_material.albedo.texture, uv).rgb + u_material.albedo.color;
    vec3 normalFromTexture = texture(u_material.normal.texture, uv).rgb + u_material.normal.color;
	normalFromTexture = normalize(normalFromTexture * 2.0 - 1.0);

    fragColor = vec4(albedo,1.0);
})";
		const char* lightingShaderVertex = R"(#version 460

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
    uv = vertexUv;
	normal = normalize(mat3(transpose(inverse(u_model))) * vertexNormal);
	fragPos = vec3(u_model * vec4(vertexPos,1.0));
    gl_Position = u_projection * u_view * u_model * vec4(vertexPos,1.0);
})";
		const char* lightingShaderFragment = R"(#version 460 

struct SubMaterial
{
    sampler2D texture;
    vec3 color;
};
struct Material 
{
    SubMaterial albedo;
    SubMaterial normal;
    SubMaterial specular;
    SubMaterial roughness;
}; 

struct DirLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float range;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float range;
    float coneAngle;
};

in vec2 uv;
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

#define MAX_COUNT_DIR_LIGHTS 16
uniform DirLight dirLights[MAX_COUNT_DIR_LIGHTS];
uniform int countDirLights;

#define MAX_COUNT_POINT_LIGHTS 16
uniform PointLight pointLights[MAX_COUNT_POINT_LIGHTS];
uniform int countPointLights;

#define MAX_COUNT_SPOT_LIGHTS 16
uniform SpotLight spotLights[MAX_COUNT_SPOT_LIGHTS];
uniform int countSpotLights;

uniform Material u_material;
uniform vec3 u_viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);


void main()
{
	vec3 viewDir = normalize(u_viewPos - fragPos);
	
	vec3 result;
	for(int i = 0; i < countDirLights; i++)
	{
  		result += CalcDirLight(dirLights[i], normal, viewDir);
	}
    for(int i = 0; i < countPointLights; i++)
	{
  		result += CalcPointLight(pointLights[i], normal, viewDir);
	}

    fragColor = vec4(result,1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    //REMOVE COLOR
	vec3 lightColor = vec3(1.0);
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * (texture(u_material.albedo.texture, uv).rgb + u_material.albedo.color) * lightColor * light.intensity;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * lightColor * light.intensity;


    return diffuse + specular; 
} 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    //REMOVE COLOR
    vec3 lightColor = vec3(1.0);
    vec3 lightDir = normalize(light.position - fragPos);

    float distanceToPoint = length(light.position - fragPos);
    if (distanceToPoint <= light.range) 
    {
        float attenuation = 1.0 / (1.0 + 0.01 * distanceToPoint + 0.032 * (distanceToPoint * distanceToPoint));

        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * (texture(u_material.albedo.texture, uv).rgb + u_material.albedo.color) * lightColor * light.intensity * attenuation;

        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = spec * lightColor * light.intensity * attenuation;    
        return diffuse + specular;
    }
    return vec3(0.0);
}
)";
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