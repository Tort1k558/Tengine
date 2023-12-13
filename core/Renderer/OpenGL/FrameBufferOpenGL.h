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
		void bindToRead() final;
		void bindToWrite() final;
		void unbind() final;

		void attachColorTexture(std::shared_ptr<Texture> texture) final;
		void attachColorMultisampleTexture(std::shared_ptr<MultisampleTexture> texture) final;
		void attachDepthTexture(std::shared_ptr<Texture> texture) final;
		void attachStencilTexture(std::shared_ptr<Texture> texture) final;

		std::shared_ptr<Texture> getColorTexture() final;
		std::shared_ptr<Texture> getDepthTexture() final;
		std::shared_ptr<Texture> getStencilTexture() final;

		static void SetDefaultBuffer();
	private:
		GLuint m_id;
		std::shared_ptr<Texture> m_colorTexture, m_depthTexture,m_stencilTexture;
		std::shared_ptr<MultisampleTexture> m_multisampledTexture;
	};
}