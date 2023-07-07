#include "Shader.h"

#include"Systems/RendererSystem.h"
#include"OpenGL/ShaderOpenGL.h"
#include<spdlog/spdlog.h>


std::shared_ptr<Shader> Shader::Create()
{
	switch (RendererSystem::GetType())
	{
	case RendererType::None:
		spdlog::critical("ERROR::Renderer not initialized!");
		return nullptr;
	case RendererType::OpenGL:
		return std::make_shared<ShaderOpenGL>();
	}
	return nullptr;
}
