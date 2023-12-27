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
		FrameBufferOpenGL();
		~FrameBufferOpenGL();
		FrameBufferOpenGL(const FrameBufferOpenGL&) = delete;
		FrameBufferOpenGL& operator=(const FrameBufferOpenGL&) = delete;
		FrameBufferOpenGL(FrameBufferOpenGL&& buffer) noexcept;
		FrameBufferOpenGL& operator=(FrameBufferOpenGL&& buffer) noexcept;

		void bind() final;
		void bindToRead() final;
		void bindToWrite() final;
		void unbind() final;
		
		void attachTexture(std::shared_ptr<Texture> texture, FrameBufferAttachment attachment) final;

		std::shared_ptr<Texture> getAttachment(FrameBufferAttachment attachment) final;

		void copy(std::shared_ptr<FrameBuffer> buffer, UVec2 src0, UVec2 src1, UVec2 dst0, UVec2 dst1, FrameBufferAttachment mask, FrameBufferCopyFilter filter) final;
		static void SetDefaultBuffer();
	private:
		GLuint m_id;
		std::unordered_map<FrameBufferAttachment,std::shared_ptr<Texture>> m_attachments;
	};
}