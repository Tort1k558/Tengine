#include "Renderer.h"
#include "OpenGL/RendererContextOpenGL.h"

std::shared_ptr<RendererContext> Renderer::m_context = std::make_shared<RendererContextOpenGL>();

void Renderer::Init()
{
	m_context->init();
}

void Renderer::OnWindowResize(unsigned int widht, unsigned int height)
{
	m_context->setViewport(0, 0, widht, height);
}

RendererType Renderer::GetType()
{
	return m_context->getType();
}
