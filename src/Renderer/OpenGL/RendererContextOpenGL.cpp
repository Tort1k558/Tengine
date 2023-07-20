#include "RendererContextOpenGL.h"

#include"Core/Logger.h"

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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::Critical("Failed to load glad!");
        return;
    }
    Logger::Info("OpenGL context");
    Logger::Info("OpenGL version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    Logger::Info("Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    Logger::Info("Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
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

void RendererContextOpenGL::setViewport(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{
    glViewport(x0, y0, x1, y1);
}

void RendererContextOpenGL::drawIndexed(std::shared_ptr<VertexArray> va)
{
    va->bind();
    glDrawElements(GL_TRIANGLES, va->getCountOfIndices(), GL_UNSIGNED_INT, 0);
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

RendererType RendererContextOpenGL::getType()
{
	return RendererType::OpenGL;
}
