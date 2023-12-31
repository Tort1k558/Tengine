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

out VS_OUT
{
    vec2 uv;
    vec3 normal;
    vec3 fragPos;
} vs_out;


void main()
{
    vs_out.uv = vertexUv;
	vs_out.normal = normalize(mat3(transpose(inverse(u_model))) * vertexNormal);
	vs_out.fragPos = vec3(u_model * vec4(vertexPos, 1.0));
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

in VS_OUT
{
    vec2 uv;
    vec3 normal;
    vec3 fragPos;
} fs_in;

out vec4 fragColor;

void main()
{
	vec3 albedo = texture(u_material.albedo.texture, fs_in.uv).rgb + u_material.albedo.color;
    fragColor = vec4(albedo,1.0);
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
    float innerConeAngle;
    float outerConeAngle;
};

in VS_OUT
{
    vec2 uv;
    vec3 normal;
    vec3 fragPos;
} fs_in;

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
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

float CalcDiffuse(vec3 normal, vec3 lightDir)
{
    return max(dot(normal, lightDir), 0.0);
}

float CalcSpecular(vec3 normal, vec3 lightDir, vec3 viewDir)
{
    vec3 halfwayDir = normalize(normalize(lightDir) + normalize(viewDir));
    return pow(max(dot(normal, halfwayDir), 0.0), 16.0);
}
void main()
{
	vec3 viewDir = normalize(u_viewPos - fs_in.fragPos);
	
	vec3 result;
	for(int i = 0; i < countDirLights; i++)
	{
  		result += CalcDirLight(dirLights[i], normalize(fs_in.normal), viewDir);
	}
    for(int i = 0; i < countPointLights; i++)
	{
  		result += CalcPointLight(pointLights[i], normalize(fs_in.normal), viewDir);
	}
    for(int i = 0; i < countSpotLights; i++)
	{
  		result += CalcSpotLight(spotLights[i], normalize(fs_in.normal), viewDir);
	}
    
    fragColor = vec4(result,1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = CalcDiffuse(normal,lightDir);
    vec3 diffuse = diff * (texture(u_material.albedo.texture, fs_in.uv).rgb + u_material.albedo.color) * light.color * light.intensity;

    float spec = CalcSpecular(normal, lightDir, viewDir);
    vec3 specular = spec * light.color * light.intensity;


    return diffuse + specular; 
} 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fs_in.fragPos);
    
    float distanceToPoint = length(light.position - fs_in.fragPos);
    if (distanceToPoint <= light.range) 
    {
        float attenuation = 1.0 / (1.0 + 0.01 * distanceToPoint + 0.032 * (distanceToPoint * distanceToPoint));

        float diff = CalcDiffuse(normal, lightDir);
        vec3 diffuse = diff * (texture(u_material.albedo.texture, fs_in.uv).rgb + u_material.albedo.color) * light.color * light.intensity * attenuation;

        float spec = CalcSpecular(normal,lightDir, viewDir);
        vec3 specular = spec * light.color * light.intensity * attenuation;    
        return diffuse + specular;
    }
    return vec3(0.0);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fs_in.fragPos);
    
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = cos(light.innerConeAngle) - cos(light.outerConeAngle);
    float intensity = clamp((theta - cos(light.outerConeAngle)) / epsilon, 0.0, 1.0); 
    if (theta > cos(light.outerConeAngle))
    {
        float distanceToPoint = length(light.position - fs_in.fragPos);
        if (distanceToPoint <= light.range) 
        {
            float attenuation = 1.0 / (1.0 + 0.01 * distanceToPoint + 0.032 * (distanceToPoint * distanceToPoint));

            float diff = CalcDiffuse(normal, lightDir);
            vec3 diffuse = diff * (texture(u_material.albedo.texture, fs_in.uv).rgb + u_material.albedo.color) * light.color * light.intensity * intensity * attenuation;

            float spec = CalcSpecular(normal,lightDir, viewDir);
            vec3 specular = spec * light.color * light.intensity * intensity * attenuation;
            return diffuse + specular;
        }
    }
    return vec3(0.0);
}
)";
const char* skyboxShaderVertex = R"(#version 460

layout(location = 0) in vec3 vertexPos;

uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 uvw;

void main()
{
    uvw = vertexPos;
    gl_Position = (u_projection * u_view * vec4(vertexPos,1.0)).xyww;
}
)";
const char* skyboxShaderFragment = R"(#version 460

in vec3 uvw;

out vec4 fragColor;

uniform samplerCube skybox;

void main()
{
    fragColor = texture(skybox,vec3(uvw.x,uvw.y,uvw.z));
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