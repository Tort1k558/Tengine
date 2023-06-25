#pragma once

#include<memory>
#include<string>
#include<vector>
#include"Renderer/Shader.h"
#include"Renderer/OpenGL/RendererContextOpenGL.h"
class ShaderOpenGL : public Shader
{
public:
	ShaderOpenGL();
	~ShaderOpenGL();

	ShaderOpenGL(const ShaderOpenGL&) = delete;
	ShaderOpenGL& operator=(const ShaderOpenGL&) = delete;
	ShaderOpenGL(ShaderOpenGL&& shader) noexcept;
	ShaderOpenGL& operator=(ShaderOpenGL&& shader) noexcept;

	void bind() override;
	void unbind() override;
	void addShader(const std::string& shaderSrc, ShaderType type) override;
	void compile() override;
	
private:
	GLuint m_id;
	std::vector<GLuint> m_shaders;
};