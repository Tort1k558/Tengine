#include "ShaderCode.h"
#include"GLSL/DefaultShader.h"
#include"GLSL/FramebufferShader.h"
namespace Tengine
{
    ShaderSource ShaderCode::GetDefaultShader()
    {
        ShaderSource source;
        source.addSourceShader(ShaderType::Vertex, GLSL::defaultShaderVertex);
        source.addSourceShader(ShaderType::Fragment, GLSL::defaultShaderFragment);
        return source;
    }

    ShaderSource ShaderCode::GetFramebufferShader()
    {
        ShaderSource source;
        source.addSourceShader(ShaderType::Vertex, GLSL::framebufferShaderVertex);
        source.addSourceShader(ShaderType::Fragment, GLSL::framebufferShaderFragment);
        return source;
    }

    void ShaderSource::addSourceShader(ShaderType type, const std::string& source)
    {
        m_source[static_cast<int>(type)] = source;
    }
    std::string ShaderSource::getSourceShader(ShaderType type)
    {
        return m_source[static_cast<int>(type)];
    }
}