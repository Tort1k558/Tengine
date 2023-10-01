#pragma once

#include<memory>

#include"Renderer/RendererContext.h"
#include"Renderer/Texture.h"
#include"ECS/System.h"
#include"Renderer/FrameBuffer.h"
namespace Tengine
{

	class TENGINE RendererSystem : public System
	{
	public:
		void init() final;
		void update() final;
		void destroy() final;
		void updateViewport(UVec2 size);
		void setRendererType(RendererType type);
		void setTextureFilter(TextureFilter filter);
		RendererType getRendererType() { return m_context->getType(); };
		TextureFilter getTextureFilter() { return m_textureFilter; }
		std::shared_ptr<FrameBuffer> getFramebuffer();
		void renderFramebuffer(std::shared_ptr<FrameBuffer> framebuffer);
		void disableRenderToDefaultFramebuffer();
		void enableRenderToDefaultFramebuffer();
		static std::shared_ptr<RendererSystem> GetInstance();
	private:
		std::unique_ptr<RendererContext> m_context;
		RendererType m_rendererType;
		UVec2 m_viewportSize;
		TextureFilter m_textureFilter = TextureFilter::Bilinear;
		std::shared_ptr<FrameBuffer> m_framebuffer;
		bool renderToDefaultFramebuffer = true;
		static std::shared_ptr<RendererSystem> m_instance;
	};
}