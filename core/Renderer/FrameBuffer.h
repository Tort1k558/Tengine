#pragma once

#include"Renderer/Texture.h"

namespace Tengine
{
	class TENGINE FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;


		virtual std::shared_ptr<Texture> getColorTexture() = 0;
		virtual std::shared_ptr<Texture> getDepthTexture() = 0;

		UVec2 getSize();

		static std::shared_ptr<FrameBuffer> Create(UVec2 size);
		static void SetDefaultBuffer();
	protected:
		UVec2 m_size;
	};
}