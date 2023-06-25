#include "ShaderOpenGL.h"

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

void ShaderOpenGL::addShader(const std::string& shaderSrc, ShaderType type)
{
    int shaderType = 0;
    switch (type)
    {
    case ShaderType::VertexShader:
        shaderType = GL_VERTEX_SHADER;
        break;
    case ShaderType::FragmentShader:
        shaderType = GL_FRAGMENT_SHADER;
        break;
    default:
        spdlog::critical("ERROR::SHADER::Unknown type of shader");
        return;
    }
    GLuint shader = glCreateShader(shaderType);

    const char* src = shaderSrc.c_str();
    glShaderSource(shader, 1, &src, nullptr);

    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        spdlog::critical("ERROR::SHADER::{0}::COMPILATION_FAILED::{1}", shaderType, infoLog);
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
        spdlog::critical("ERROR::SHADER::COMPILATION_FAILED::{0}", infoLog);
        return;
    }
    for (const auto& shader :m_shaders)
    {
        glDetachShader(m_id, shader);
        glDeleteShader(shader);
    }
}
