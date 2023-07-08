#include "Shader.h"

#include"Systems/RendererSystem.h"
#include"OpenGL/ShaderOpenGL.h"
#include"Core/Logger.h"


std::shared_ptr<Shader> Shader::Create()
{
	switch (RendererSystem::GetType())
	{
	case RendererType::None:
		Logger::Critical("ERROR::Renderer not initialized!");
		return nullptr;
	case RendererType::OpenGL:
		return std::make_shared<ShaderOpenGL>();
	}
	return nullptr;
}
