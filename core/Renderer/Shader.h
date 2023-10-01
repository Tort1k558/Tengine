#pragma once

#include<memory>
#include<string>
#include"Core/Math.h"
#include"Core/AssetManager.h"

namespace Tengine
{
	enum class ShaderType
	{
		Vertex = 0,
		Fragment
	};

	class Shader : public Resource
	{
	public:
		virtual ~Shader() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual void addShader(std::string_view shaderSrc, ShaderType type) = 0;
		virtual void compile() = 0;

		virtual void setUniformInt(std::string_view name, const int value) = 0;
		virtual void setUniformFloat(std::string_view name, const float value) = 0;
		virtual void setUniformVec2(std::string_view name, const Vec2& vec) = 0;
		virtual void setUniformVec3(std::string_view name, const Vec3& vec) = 0;
		virtual void setUniformVec4(std::string_view name, const Vec4& vec) = 0;
		virtual void setUniformMat2(std::string_view name, const Mat2& mat) = 0;
		virtual void setUniformMat3(std::string_view name, const Mat3& mat) = 0;
		virtual void setUniformMat4(std::string_view name, const Mat4& mat) = 0;
		static std::shared_ptr<Shader> Create();
	};
}