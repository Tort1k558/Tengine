#pragma once

#include"Renderer/RendererContext.h"
#include"ECS/System.h"
#include<memory>
class RendererSystem : public System
{
public:
	void init() final;
	void update() final;
	void destroy() final;
	void updateWindowSize(unsigned int widht,unsigned int height);
	void setRendererType(RendererType type);
	RendererType getType();
private:
	std::unique_ptr<RendererContext> m_context;
	RendererType m_rendererType;
};