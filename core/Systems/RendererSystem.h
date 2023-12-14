#pragma once

#include<memory>

#include"Renderer/RendererContext.h"
#include"Renderer/Texture.h"
#include"Scene/System.h"
#include"Renderer/FrameBuffer.h"
#include"Components/Camera.h"


namespace Tengine
{

	class TENGINE RendererSystem : public System
	{
	public:
		void init() final;
		void update() final;
		void destroy() final;

		void updateViewport(UVec2 size);
		UVec2 getViewport();
		void setRendererType(RendererType type);
		void setTextureFilter(TextureFilter filter);
		RendererType getRendererType();
		TextureFilter getTextureFilter();
		void renderFramebuffer(std::shared_ptr<FrameBuffer> framebuffer);
		void renderFramebufferToFramebuffer(std::shared_ptr<FrameBuffer> srcFramebuffer, std::shared_ptr<FrameBuffer> dstFramebuffer);
		void disableRenderToDefaultFramebuffer();
		void enableRenderToDefaultFramebuffer();

		void renderCamera(std::shared_ptr<Camera> camera);

		std::shared_ptr<RendererContext> getRendererContext();
		static std::shared_ptr<RendererSystem> GetInstance();
	private:
		std::shared_ptr<RendererContext> m_context;
		RendererType m_rendererType = RendererType::None;
		UVec2 m_viewportSize;
		TextureFilter m_textureFilter = TextureFilter::Bilinear;
		bool renderToDefaultFramebuffer = true;

		static std::shared_ptr<RendererSystem> m_instance;
	};
}