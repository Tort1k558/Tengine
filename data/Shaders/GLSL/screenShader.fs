#version 460 

layout (binding = 0)uniform sampler2D ourTexture;

in vec2 uv;
out vec4 fragColor;

void main()
{
    fragColor = texture(ourTexture,uv);
}