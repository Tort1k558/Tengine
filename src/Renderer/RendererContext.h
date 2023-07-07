#pragma once
#include"VertexArray.h"
#include<memory>

enum class RendererType;

enum class RendererType
{
	None = 0,
	OpenGL
};

class RendererContext
{
public:
	virtual ~RendererContext() = default;

	virtual void init() = 0;
	virtual void setViewport(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) = 0;
	virtual void drawIndexed(std::shared_ptr<VertexArray> va) = 0;
	virtual RendererType getType() = 0;
};