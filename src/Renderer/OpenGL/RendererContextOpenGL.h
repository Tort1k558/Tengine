#pragma once
#include"../RendererContext.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<memory>


class RendererContextOpenGL : public RendererContext
{
public:
	void init() override;
	void setViewport(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) override;
	void drawIndexed(std::shared_ptr<VertexArray> va) override;
	RendererType getType() override;
};