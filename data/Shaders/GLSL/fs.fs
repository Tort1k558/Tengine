#version 460 

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
}