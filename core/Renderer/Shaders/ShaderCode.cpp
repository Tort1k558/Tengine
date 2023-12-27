#include "ShaderCode.h"
#include"GLSL/DefaultShader.h"
#include"GLSL/FramebufferShader.h"
#include"Systems/RendererSystem.h"

#include"Core/Logger.h"

namespace Tengine
{
    ShaderSource ShaderCode::GetDefaultShader()
    {
        ShaderSource source;
        switch (RendererSystem::GetInstance()->getRendererType())
        {
        case RendererType::OpenGL:
            source.addSourceShader(ShaderType::Vertex, GLSL::defaultShaderVertex);
            source.addSourceShader(ShaderType::Fragment, GLSL::defaultShaderFragment);
            break;
        case RendererType::OpenGLES:
            source.addSourceShader(ShaderType::Vertex, ESSL::defaultShaderVertex);
            source.addSourceShader(ShaderType::Fragment, ESSL::defaultShaderFragment);
            break;
        default:
            Logger::Critical("ERROR::ShaderCode::CurrentShader doesn't support in this RenderContext!");
            break;
        }
        return source;
    }

    ShaderSource ShaderCode::GetLightingShader()
    {
        ShaderSource source;
        switch (RendererSystem::GetInstance()->getRendererType())
        {
        case RendererType::OpenGL:
            source.addSourceShader(ShaderType::Vertex, GLSL::defaultShaderVertex);
            source.addSourceShader(ShaderType::Fragment, GLSL::lightingShaderFragment);
            break;
        default:
            Logger::Critical("ERROR::ShaderCode::CurrentShader doesn't support in this RenderContext!");
            break;
        }
        return source;
    }

    ShaderSource ShaderCode::GetFramebufferShader()
    {
        ShaderSource source;
        switch (RendererSystem::GetInstance()->getRendererType())
        {
        case RendererType::OpenGL:
            source.addSourceShader(ShaderType::Vertex, GLSL::framebufferShaderVertex);
            source.addSourceShader(ShaderType::Fragment, GLSL::framebufferShaderFragment);
            break;
        case RendererType::OpenGLES:
            source.addSourceShader(ShaderType::Vertex, ESSL::framebufferShaderVertex);
            source.addSourceShader(ShaderType::Fragment, ESSL::framebufferShaderFragment);
            break;
        default:
            Logger::Critical("ERROR::ShaderCode::CurrentShader doesn't support in this RenderContext!");
            break;
        }
        return source;
    }

    ShaderSource ShaderCode::GetPostProcessingShader()
    {
        ShaderSource source;
        switch (RendererSystem::GetInstance()->getRendererType())
        {
        case RendererType::OpenGL:
            source.addSourceShader(ShaderType::Vertex, GLSL::framebufferShaderVertex);
            source.addSourceShader(ShaderType::Fragment, GLSL::postProcessingShaderFragment);
            break;
        case RendererType::OpenGLES:
            //source.addSourceShader(ShaderType::Vertex, ESSL::framebufferShaderVertex);
            //source.addSourceShader(ShaderType::Fragment, ESSL::PostProcessingShaderFragment);
            break;
        default:
            Logger::Critical("ERROR::ShaderCode::CurrentShader doesn't support in this RenderContext!");
            break;
        }
        return source;
    }

    ShaderSource ShaderCode::GetSkyboxShader()
    {
        ShaderSource source;
        switch (RendererSystem::GetInstance()->getRendererType())
        {
        case RendererType::OpenGL:
            source.addSourceShader(ShaderType::Vertex, GLSL::skyboxShaderVertex);
            source.addSourceShader(ShaderType::Fragment, GLSL::skyboxShaderFragment);
            break;
        case RendererType::OpenGLES:
            //
            break;
        default:
            Logger::Critical("ERROR::ShaderCode::CurrentShader doesn't support in this RenderContext!");
            break;
        }
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