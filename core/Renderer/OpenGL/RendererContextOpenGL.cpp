#include "RendererContextOpenGL.h"

#include"Core/Logger.h"
#include"Core/Core.h"

namespace Tengine
{

    std::string GlSourceToStr(const GLenum source)
    {
        switch (source)
        {
        case GL_DEBUG_SOURCE_API: return "DEBUG_SOURCE_API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "DEBUG_SOURCE_WINDOW_SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "GL_DEBUG_SOURCE_SHADER_COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "DEBUG_SOURCE_THIRD_PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: return "DEBUG_SOURCE_APPLICATION";
        case GL_DEBUG_SOURCE_OTHER: return "DEBUG_SOURCE_OTHER";

        default: return "UNKNOWN_DEBUG_SOURCE";
        }
    }

    std::string GlTypeToStr(const GLenum type)
    {
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR: return "DEBUG_TYPE_ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEBUG_TYPE_DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "DEBUG_TYPE_UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: return "DEBUG_TYPE_PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: return "DEBUG_TYPE_PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: return "DEBUG_TYPE_MARKER";
        case GL_DEBUG_TYPE_PUSH_GROUP: return "DEBUG_TYPE_PUSH_GROUP";
        case GL_DEBUG_TYPE_POP_GROUP: return "DEBUG_TYPE_POP_GROUP";
        case GL_DEBUG_TYPE_OTHER: return "DEBUG_TYPE_OTHER";

        default: return "UNKNOWN_DEBUG_TYPE";
        }
    }

    void RendererContextOpenGL::init()
    {
        m_type = RendererType::OpenGL;

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Logger::Critical("Failed to load glad!");
            return;
        }
        Logger::Info("OpenGL context");
        std::string version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        if (version.find("OpenGL ES") != std::string::npos)
        {
            m_type = RendererType::OpenGLES;
        }
        Logger::Info("OpenGL version: {0}", version);
        Logger::Info("Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        Logger::Info("Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    }

    void RendererContextOpenGL::setViewport(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
    {
        glViewport(x0, y0, x1, y1);
    }

    void RendererContextOpenGL::drawIndexed(std::shared_ptr<VertexArray> vertexArray)
    {
        vertexArray->bind();
        glDrawElements(GL_TRIANGLES, vertexArray->getCountOfIndices(), GL_UNSIGNED_INT, 0);
    }

    void RendererContextOpenGL::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RendererContextOpenGL::clearColor(Vec4 color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void RendererContextOpenGL::enableDepthTest()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void RendererContextOpenGL::disableDepthTest()
    {
        glDisable(GL_DEPTH_TEST);
    }

    void RendererContextOpenGL::enableDebugInfo()
    {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
        glDebugMessageCallback([](GLenum glSource, GLenum glType, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
            {
                std::string source = GlSourceToStr(glSource);
                std::string type = GlTypeToStr(glType);
                switch (severity)
                {
                case GL_DEBUG_SEVERITY_HIGH:
                    Logger::Critical("ERROR::OpenGL::{0}::{1}::{2}:: {3}", source, type, id, message);
                    break;
                case GL_DEBUG_SEVERITY_MEDIUM:
                case GL_DEBUG_SEVERITY_LOW:
                    Logger::Warning("WARNING::OpenGL::{0}::{1}::{2}:: {3}", source, type, id, message);
                    break;
                case GL_DEBUG_SEVERITY_NOTIFICATION:
                    Logger::Info("NOTIFICATION::OpenGL::{0}::{1}::{2}:: {3}", source, type, id, message);
                    break;
                default:
                    Logger::Critical("ERROR::OpenGL::{0}::{1}::{2}:: {3}", source, type, id, message);
                }
            }, nullptr);
    }
    void RendererContextOpenGL::setDepthFunc(DepthFunc func)
    {
        switch (func)
        {
        case Tengine::DepthFunc::Always:
            glDepthFunc(GL_ALWAYS);
            break;
        case Tengine::DepthFunc::Never:
            glDepthFunc(GL_NEVER);
            break;
        case Tengine::DepthFunc::Less:
            glDepthFunc(GL_LESS);
            break;
        case Tengine::DepthFunc::Equal:
            glDepthFunc(GL_EQUAL);
            break;
        case Tengine::DepthFunc::LessOrEqual:
            glDepthFunc(GL_LEQUAL);
            break;
        case Tengine::DepthFunc::Greater:
            glDepthFunc(GL_GREATER);
            break;
        case Tengine::DepthFunc::NotEqual:
            glDepthFunc(GL_NOTEQUAL);
            break;
        case Tengine::DepthFunc::GreaterOrEqual:
            glDepthFunc(GL_GEQUAL);
            break;
        default:
            break;
        }
    }
}