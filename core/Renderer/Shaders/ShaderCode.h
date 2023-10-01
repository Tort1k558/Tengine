#pragma once

#include<array>
#include<string>

#include"Renderer/Shader.h"
namespace Tengine
{
	class ShaderSource
	{
	public:
		ShaderSource() = default;
		void addSourceShader(ShaderType type, std::string_view source);
		std::string getSourceShader(ShaderType type);
	private:
		std::array<std::string,static_cast<int>(ShaderType::Fragment)+1> m_source;
	};
	class ShaderCode
	{
	public:
		ShaderCode() = delete;

		static ShaderSource GetDefaultShader();
		static ShaderSource GetFramebufferShader();
	};
}