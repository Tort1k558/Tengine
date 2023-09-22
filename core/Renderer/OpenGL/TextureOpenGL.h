#pragma once

#include"Renderer/Texture.h"
#include"Renderer/OpenGL/RendererContextOpenGL.h"
#include"FrameBufferOpenGL.h"

namespace Tengine
{
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
		void* getData() final;
		unsigned int getId() final;
	private:
		friend class FrameBufferOpenGL;
		GLuint m_id;
	};
}