#pragma once

#include "Renderer/Texture.h"
#include"Renderer/OpenGL/RendererContextOpenGL.h"

class TextureOpenGL : public Texture
{
public:
	TextureOpenGL(void *data, UVec2 size, TextureType type);
	~TextureOpenGL();

	void bind(unsigned int slot) final;

private:
	GLuint m_id;
};