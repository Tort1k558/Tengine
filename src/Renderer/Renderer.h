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
	static void Render();
	static void UpdateWindowSize(unsigned int widht,unsigned int height);
	static RendererType GetType();
private:
	static std::unique_ptr<RendererContext> m_context;
};