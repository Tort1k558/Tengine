#pragma once

#include"Renderer/RendererContext.h"
#include<memory>
class RendererSystem
{
public:
	RendererSystem() = delete;
	RendererSystem(const RendererSystem&) = delete;
	RendererSystem& operator=(const RendererSystem&) = delete;

	static void Init();
	static void Update();
	static void UpdateWindowSize(unsigned int widht,unsigned int height);
	static RendererType GetType();
private:
	static std::unique_ptr<RendererContext> m_context;
};