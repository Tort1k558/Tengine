#pragma once

#include"Renderer/Texture.h"
#include"Renderer/OpenGL/RendererContextOpenGL.h"
#include"FrameBufferOpenGL.h"

namespace Tengine
{
	GLenum TextureTypeToOpenGLFormat(TextureType type);
	GLenum TextureTypeToOpenGLInternalFormat(TextureType type);
	TextureType OpenGLInternalFormatToTextureType(GLint type);
	GLenum TextureTypeToOpenGLType(TextureType type);

	class TextureOpenGL : public Texture
	{
	public:
		TextureOpenGL(void* data, UVec2 size, TextureType type,TextureFilter filter = TextureFilter::None);
		~TextureOpenGL();
		TextureOpenGL(const TextureOpenGL&) = delete;
		TextureOpenGL& operator=(const TextureOpenGL&) = delete;
		TextureOpenGL(TextureOpenGL&& texture) noexcept;
		TextureOpenGL& operator=(TextureOpenGL&& texture) noexcept;

		void bind(unsigned int slot) final;
		std::shared_ptr<void> getData() final;
		unsigned int getId() final;
	private:
		GLuint m_id;

		friend class FrameBufferOpenGL;
	};

	class MultisampleTextureOpenGL : public MultisampleTexture
	{
	public:
		MultisampleTextureOpenGL(void* data, UVec2 size, TextureType type, size_t samples);
		~MultisampleTextureOpenGL();
		MultisampleTextureOpenGL(const MultisampleTextureOpenGL&) = delete;
		MultisampleTextureOpenGL& operator=(const MultisampleTextureOpenGL&) = delete;
		MultisampleTextureOpenGL(MultisampleTextureOpenGL&& texture) noexcept;
		MultisampleTextureOpenGL& operator=(MultisampleTextureOpenGL&& texture) noexcept;

		void bind(unsigned int slot) final;
		unsigned int getId() final;
		std::shared_ptr<void> getData() final;
	private:
		GLuint m_id;
	};

}