#pragma once

#include<memory>
#include<string>
#include<vector>
#include<unordered_map>

#include"Renderer/Shader.h"
#include"Renderer/OpenGL/RendererContextOpenGL.h"

namespace Tengine
{
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

		void setUniformInt(std::string_view name, const int value) override;
		void setUniformFloat(std::string_view name, const float value) override;
		void setUniformVec2(std::string_view name, const Vec2& vec) override;
		void setUniformVec3(std::string_view name, const Vec3& vec) override;
		void setUniformVec4(std::string_view name, const Vec4& vec) override;
		void setUniformMat2(std::string_view name, const Mat2& mat) override;
		void setUniformMat3(std::string_view name, const Mat3& mat) override;
		void setUniformMat4(std::string_view name, const Mat4& mat) override;

	private:
		GLuint findUniformLocation(std::string_view name);
		GLuint m_id;
		std::vector<GLuint> m_shaders;
		std::unordered_map<std::string, GLuint> m_uniformLocations;
	};
}