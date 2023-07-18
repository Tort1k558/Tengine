#version 460 

layout (binding = 0)uniform sampler2D ourTexture;

in  vec2 uv;
out vec4 fragColor;

void main()
{
	//fragColor = vec4(uv.r, uv.g, 0.0,1.0);
    fragColor = texture(ourTexture,uv);
}