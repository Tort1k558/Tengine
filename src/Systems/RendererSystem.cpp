#include "RendererSystem.h"
#include "Renderer/OpenGL/RendererContextOpenGL.h"

std::unique_ptr<RendererContext> RendererSystem::m_context = std::make_unique<RendererContextOpenGL>();

void RendererSystem::Init()
{
	m_context->init();
}

void RendererSystem::Update()
{
	m_context->clearColor({ 0.0f,0.0f,0.0f, 1.0f});
	m_context->clear();

}

void RendererSystem::UpdateWindowSize(unsigned int widht, unsigned int height)
{
	m_context->setViewport(0, 0, widht, height);
}

RendererType RendererSystem::GetType()
{
	return m_context->getType();
}
