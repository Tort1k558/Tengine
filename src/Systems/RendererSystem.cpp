#include "RendererSystem.h"

#include "Renderer/OpenGL/RendererContextOpenGL.h"
#include "Core/Logger.h"

void RendererSystem::init()
{
	switch (m_rendererType)
	{
	case RendererType::None:
		Logger::Critical("ERROR::Renderer type not specified!");
		return;
	case RendererType::OpenGL:
		{
			m_context = std::make_unique<RendererContextOpenGL>();
		}
	}
	m_context->init();
}

void RendererSystem::update()
{
	m_context->clearColor({ 0.0f,0.0f,0.0f, 1.0f});
	m_context->clear();

}

void RendererSystem::updateWindowSize(unsigned int widht, unsigned int height)
{
	m_context->setViewport(0, 0, widht, height);
}

void RendererSystem::setRendererType(RendererType type)
{
	m_rendererType = type;
}

void RendererSystem::destroy()
{

}

RendererType RendererSystem::getType()
{
	return m_context->getType();
}
