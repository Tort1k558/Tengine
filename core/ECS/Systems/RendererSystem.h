#pragma once

#include<memory>

#include"Renderer/RendererContext.h"
#include"Renderer/Texture.h"
#include"ECS/System.h"

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

		static std::shared_ptr<RendererSystem> GetInstance();
	private:
		std::unique_ptr<RendererContext> m_context;
		RendererType m_rendererType;
		UVec2 m_viewportSize;
		TextureFilter m_textureFilter = TextureFilter::Bilinear;

		static std::shared_ptr<RendererSystem> m_instance;
	};
}