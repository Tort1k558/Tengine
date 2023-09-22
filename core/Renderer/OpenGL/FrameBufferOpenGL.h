#pragma once

#include"Renderer/FrameBuffer.h"
#include"Renderer/OpenGL/RendererContextOpenGL.h"
#include"TextureOpenGL.h"


namespace Tengine
{
	class TextureOpenGL;

	class FrameBufferOpenGL : public FrameBuffer
	{
	public:
		FrameBufferOpenGL(UVec2 size);
		~FrameBufferOpenGL();
		FrameBufferOpenGL(const FrameBufferOpenGL&) = delete;
		FrameBufferOpenGL& operator=(const FrameBufferOpenGL&) = delete;
		FrameBufferOpenGL(FrameBufferOpenGL&& buffer) noexcept;
		FrameBufferOpenGL& operator=(FrameBufferOpenGL&& buffer) noexcept;

		void bind() final;
		void unbind() final;


		std::shared_ptr<Texture> getColorTexture() final;
		std::shared_ptr<Texture> getDepthTexture() final;
	private:
		GLuint m_id;
		std::shared_ptr<TextureOpenGL> m_colorTexture;
		std::shared_ptr<TextureOpenGL> m_depthTexture;
	};
}