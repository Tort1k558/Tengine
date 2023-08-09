#include "Shader.h"

#include"ECS/Systems/RendererSystem.h"
#include"OpenGL/ShaderOpenGL.h"
#include"Core/Logger.h"

namespace Tengine
{

	std::shared_ptr<Shader> Shader::Create()
	{
		switch (System::GetInstance<RendererSystem>()->getRendererType())
		{
		case RendererType::None:
			Logger::Critical("ERROR::Renderer not initialized!");
			return nullptr;
		case RendererType::OpenGL:
			return std::make_shared<ShaderOpenGL>();
		}
		return nullptr;
	}
}