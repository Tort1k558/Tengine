#pragma once

#include"Renderer/Texture.h"
#include"Renderer/OpenGL/RendererContextOpenGL.h"
#include"FrameBufferOpenGL.h"

namespace Tengine
{
	GLenum TextureTypeToOpenGLFormat(TextureType type);
	GLenum TextureTypeToOpenGLInternalFormat(TextureType type);
	GLenum TextureTypeToOpenGLType(TextureType type);

	class TextureOpenGL : public Texture
	{
	public:
		TextureOpenGL(std::shared_ptr<Image> image, TextureType type,TextureFilter filter = TextureFilter::None);
		TextureOpenGL(void* data, UVec2 size, TextureType type,TextureFilter filter = TextureFilter::None);
		~TextureOpenGL();
		TextureOpenGL(const TextureOpenGL&) = delete;
		TextureOpenGL& operator=(const TextureOpenGL&) = delete;
		TextureOpenGL(TextureOpenGL&& texture) noexcept;
		TextureOpenGL& operator=(TextureOpenGL&& texture) noexcept;

		void bind(unsigned int slot) final;
		void* getData() final;
		unsigned int getId() final;
	private:
		void generateTexture(void* data, UVec2 size);
		GLuint m_id;

		friend class FrameBufferOpenGL;
	};

}