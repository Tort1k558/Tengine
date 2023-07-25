#pragma once

#include<memory>
#include<string>
#include<vector>
#include<unordered_map>

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
	void addShader(std::string_view shaderSrc, ShaderType type) override;
	void compile() override;

	void setUniformInt(const std::string& name, const int value) override;
	void setUniformFloat(const std::string& name, const float value) override;
	void setUniformVec2(const std::string& name, const Vec2& vec) override;
	void setUniformVec3(const std::string& name, const Vec3& vec) override;
	void setUniformVec4(const std::string& name, const Vec4& vec) override;
	void setUniformMat2(const std::string& name, const Mat2& mat) override;
	void setUniformMat3(const std::string& name, const Mat3& mat) override;
	void setUniformMat4(const std::string& name, const Mat4& mat) override;
	
private:
	GLuint findUniformLocation(const std::string& name);
	GLuint m_id;
	std::vector<GLuint> m_shaders;
	std::unordered_map<std::string, GLuint> m_uniformLocations;
};