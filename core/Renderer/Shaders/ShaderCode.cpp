#include "ShaderCode.h"
#include"GLSL/DefaultShader.h"
#include"GLSL/FramebufferShader.h"
namespace Tengine
{
    ShaderSource ShaderCode::GetDefaultShader()
    {
        ShaderSource source;
        source.addSourceShader(ShaderType::Vertex, ESSL::defaultShaderVertex);
        source.addSourceShader(ShaderType::Fragment, ESSL::defaultShaderFragment);
        return source;
    }

    ShaderSource ShaderCode::GetFramebufferShader()
    {
        ShaderSource source;
        source.addSourceShader(ShaderType::Vertex, ESSL::framebufferShaderVertex);
        source.addSourceShader(ShaderType::Fragment, ESSL::framebufferShaderFragment);
        return source;
    }

    void ShaderSource::addSourceShader(ShaderType type, std::string_view source)
    {
        m_source[static_cast<int>(type)] = source;
    }
    std::string ShaderSource::getSourceShader(ShaderType type)
    {
        return m_source[static_cast<int>(type)];
    }
}