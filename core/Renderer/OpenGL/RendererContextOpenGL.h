#pragma once
#include"../RendererContext.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<memory>

namespace Tengine
{

	class RendererContextOpenGL : public RendererContext
	{
	public:
		void init() final;
		void setViewport(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) final;
		void drawIndexed(std::shared_ptr<VertexArray> vertexArray) final;
		void clear() final;
		void clearColor(Vec4 color) final;
		void enableDepthTest() final;
		void disableDepthTest() final;
		RendererType getType() final;
	};
}