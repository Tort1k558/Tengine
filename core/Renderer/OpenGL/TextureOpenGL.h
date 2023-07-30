#pragma once

#include "Renderer/Texture.h"
#include"Renderer/OpenGL/RendererContextOpenGL.h"

class TextureOpenGL : public Texture
{
public:
	TextureOpenGL(void *data, UVec2 size, TextureType type);
	~TextureOpenGL();
	TextureOpenGL(const TextureOpenGL&) = delete;
	TextureOpenGL& operator=(const TextureOpenGL&) = delete;
	TextureOpenGL(TextureOpenGL&& texture) noexcept;
	TextureOpenGL& operator=(TextureOpenGL&& texture) noexcept;

	void bind(unsigned int slot) final;
	intptr_t getId() final;
private:
	GLuint m_id;
};