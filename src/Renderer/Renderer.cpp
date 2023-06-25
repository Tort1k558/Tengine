#include "Renderer.h"
#include "OpenGL/RendererContextOpenGL.h"

std::unique_ptr<RendererContext> Renderer::m_context = std::make_unique<RendererContextOpenGL>();

void Renderer::Init()
{
	m_context->init();
}

void Renderer::Render()
{
	
}

void Renderer::UpdateWindowSize(unsigned int widht, unsigned int height)
{
	m_context->setViewport(0, 0, widht, height);
}

RendererType Renderer::GetType()
{
	return m_context->getType();
}
