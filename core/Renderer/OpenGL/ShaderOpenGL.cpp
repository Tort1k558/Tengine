#include "ShaderOpenGL.h"

#include "Core/Logger.h"

namespace Tengine
{

    ShaderOpenGL::ShaderOpenGL()
    {
        m_id = glCreateProgram();
    }

    ShaderOpenGL::~ShaderOpenGL()
    {
        glDeleteProgram(m_id);
    }

    ShaderOpenGL::ShaderOpenGL(ShaderOpenGL&& shader) noexcept
    {
        m_id = shader.m_id;
        shader.m_id = 0;
    }

    ShaderOpenGL& ShaderOpenGL::operator=(ShaderOpenGL&& shader) noexcept
    {
        glDeleteProgram(m_id);
        m_id = shader.m_id;
        shader.m_id = 0;
        return *this;
    }

    void ShaderOpenGL::bind()
    {
        glUseProgram(m_id);
    }

    void ShaderOpenGL::unbind()
    {
        glUseProgram(0);
    }

    void ShaderOpenGL::addShader(std::string_view shaderSrc, ShaderType type)
    {
        int shaderType = 0;
        switch (type)
        {
        case ShaderType::Vertex:
            shaderType = GL_VERTEX_SHADER;
            break;
        case ShaderType::Fragment:
            shaderType = GL_FRAGMENT_SHADER;
            break;
        default:
            Logger::Critical("ERROR::OpenGL::SHADER::Unknown type of shader");
            return;
        }
        GLuint shader = glCreateShader(shaderType);

        const char* src = shaderSrc.data();
        glShaderSource(shader, 1, &src, nullptr);

        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[1024];
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            Logger::Critical("ERROR::OpenGL::SHADER::{0}::COMPILATION_FAILED::{1}", shaderType, infoLog);
            return;
        }

        glAttachShader(m_id, shader);
        m_shaders.push_back(shader);
    }

    void ShaderOpenGL::compile()
    {
        glLinkProgram(m_id);
        int success;
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[1024];
            glGetProgramInfoLog(m_id, 1024, nullptr, infoLog);
            Logger::Critical("ERROR::OpenGL::SHADER::COMPILATION_FAILED::{0}", infoLog);
            return;
        }
        for (const auto& shader : m_shaders)
        {
            glDetachShader(m_id, shader);
            glDeleteShader(shader);
        }
    }

    void ShaderOpenGL::setUniformInt(std::string_view name, const int value)
    {
        bind();
        GLuint location = glGetUniformLocation(m_id, name.data());
        glUniform1i(location, value);
    }

    void ShaderOpenGL::setUniformFloat(std::string_view name, const float value)
    {
        bind();
        GLuint location = findUniformLocation(name.data());
        glUniform1f(location, value);
    }

    void ShaderOpenGL::setUniformVec2(std::string_view name, const Vec2& vec)
    {
        bind();
        GLuint location = findUniformLocation(name.data());
        glUniform2f(location, vec[0], vec[1]);
    }

    void ShaderOpenGL::setUniformVec3(std::string_view name, const Vec3& vec)
    {
        bind();
        GLuint location = findUniformLocation(name.data());
        glUniform3f(location, vec[0], vec[1], vec[2]);
    }

    void ShaderOpenGL::setUniformVec4(std::string_view name, const Vec4& vec)
    {
        bind();
        GLuint location = findUniformLocation(name.data());
        glUniform4f(location, vec[0], vec[1], vec[2], vec[3]);
    }

    void ShaderOpenGL::setUniformMat2(std::string_view name, const Mat2& mat)
    {
        bind();
        GLuint location = findUniformLocation(name.data());
        glUniformMatrix2fv(location, 1, GL_FALSE, &(mat[0].x));
    }

    void ShaderOpenGL::setUniformMat3(std::string_view name, const Mat3& mat)
    {
        bind();
        GLuint location = findUniformLocation(name.data());
        glUniformMatrix3fv(location, 1, GL_FALSE, &(mat[0].x));
    }

    void ShaderOpenGL::setUniformMat4(std::string_view name, const Mat4& mat)
    {
        bind();
        GLuint location = findUniformLocation(name.data());
        glUniformMatrix4fv(location, 1, GL_FALSE, &(mat[0].x));
    }

    GLuint ShaderOpenGL::findUniformLocation(std::string_view name)
    {
        auto it = m_uniformLocations.find(name.data());
        if (it != m_uniformLocations.end())
        {
            return it->second;
        }
        m_uniformLocations[name.data()] = glGetUniformLocation(m_id, name.data());

        return m_uniformLocations[name.data()];
    }
}