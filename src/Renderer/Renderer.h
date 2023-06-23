#pragma once

#include"RendererContext.h"
#include<memory>
enum class RendererType
{
	None = 0,
	OpenGL
};

class Renderer
{
public:
	static void Init();
	static void OnWindowResize(unsigned int widht,unsigned int height);
	static RendererType GetType();
private:
	static std::shared_ptr<RendererContext> m_context;
};