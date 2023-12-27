#pragma once

#include"Renderer/Texture.h"

namespace Tengine
{
	enum class FrameBufferAttachment
	{
		Color = 0,
		Depth,
		Stencil,
		DepthStencil
	};
	enum class FrameBufferCopyFilter
	{
		Linear = 0,
		Nearest,
	};
	class TENGINE FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void bind() = 0;
		virtual void bindToRead() = 0;
		virtual void bindToWrite() = 0;
		virtual void unbind() = 0;

		
		virtual void attachTexture(std::shared_ptr<Texture> texture, FrameBufferAttachment attachment) = 0;
		virtual std::shared_ptr<Texture> getAttachment(FrameBufferAttachment attachment) = 0;

		virtual void copy(std::shared_ptr<FrameBuffer> buffer,UVec2 src0, UVec2 src1, UVec2 dst0, UVec2 dst1, FrameBufferAttachment mask, FrameBufferCopyFilter filter) = 0;

		static std::shared_ptr<FrameBuffer> Create();
		static void SetDefaultBuffer();
	protected:
	};
}