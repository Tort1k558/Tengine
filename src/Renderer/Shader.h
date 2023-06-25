#pragma once

#include<memory>
#include<string>

enum class ShaderType
{
	VertexShader = 0,
	FragmentShader
};

class Shader
{
public:
	virtual ~Shader() = default;

	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void addShader(const std::string& shaderSrc, ShaderType type) = 0;
	virtual void compile() = 0;

	static std::shared_ptr<Shader> Create();
};