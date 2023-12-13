#pragma once

#include"Renderer/Texture.h"

namespace Tengine
{
	enum class FrameBufferAttachment
	{
		Color = 0,
		Depth,
		Stencil
	};
	class TENGINE FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void bind() = 0;
		virtual void bindToRead() = 0;
		virtual void bindToWrite() = 0;
		virtual void unbind() = 0;

		virtual void attachColorTexture(std::shared_ptr<Texture> texture) = 0;
		virtual void attachColorMultisampleTexture(std::shared_ptr<MultisampleTexture> texture) = 0;
		virtual void attachDepthTexture(std::shared_ptr<Texture> texture) = 0;
		virtual void attachStencilTexture(std::shared_ptr<Texture> texture) = 0;

		virtual std::shared_ptr<Texture> getColorTexture() = 0;
		virtual std::shared_ptr<Texture> getDepthTexture() = 0;
		virtual std::shared_ptr<Texture> getStencilTexture() = 0;

		virtual void copy(UVec2 src0, UVec2 src1, FrameBufferAttachment mask) = 0;
		UVec2 getSize();

		static std::shared_ptr<FrameBuffer> Create(UVec2 size);
		static void SetDefaultBuffer();
	protected:
		UVec2 m_size;
	};
}