#pragma once

#include<memory>
#include<string>
#include"Core/Math.h"
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

	virtual void setUniformInt(const std::string& name, const int value) = 0;
	virtual void setUniformFloat(const std::string& name, const float value) = 0;
	virtual void setUniformVec2(const std::string& name, const Vec2& vec) = 0;
	virtual void setUniformVec3(const std::string& name, const Vec3& vec) = 0;
	virtual void setUniformVec4(const std::string& name, const Vec4& vec) = 0;
	virtual void setUniformMat2(const std::string& name, const Mat2& mat) = 0;
	virtual void setUniformMat3(const std::string& name, const Mat3& mat) = 0;
	virtual void setUniformMat4(const std::string& name, const Mat4& mat) = 0;
	static std::shared_ptr<Shader> Create();
};