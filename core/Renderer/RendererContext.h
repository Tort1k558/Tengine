#pragma once

#include<memory>

#include"VertexArray.h"
#include"Core/Math.h"

namespace Tengine
{

	enum class RendererType
	{
		None = 0,
		OpenGL,
		OpenGLES
	};

	class RendererContext
	{
	public:
		virtual ~RendererContext() = default;

		virtual void init() = 0;
		virtual void setViewport(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) = 0;
		virtual void drawIndexed(std::shared_ptr<VertexArray> va) = 0;
		virtual void clear() = 0;
		virtual void clearColor(Vec4 color) = 0;
		virtual void enableDepthTest() = 0;
		virtual void disableDepthTest() = 0;
		virtual void enableDebugInfo() = 0;
		virtual RendererType getType() = 0;
	};
}