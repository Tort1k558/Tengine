#include "RendererContextOpenGL.h"
#include "Renderer/Renderer.h"
void RendererContextOpenGL::init()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::critical("Failed to load glad!");
        return;
    }
    spdlog::info("OpenGL context");
    spdlog::info("OpenGL version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    spdlog::info("Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    spdlog::info("Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

}

void RendererContextOpenGL::setViewport(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{
    glViewport(x0, y0, x1, y1);
}

void RendererContextOpenGL::drawIndexed(std::shared_ptr<VertexArray> va)
{

}

RendererType RendererContextOpenGL::getType()
{
	return RendererType::OpenGL;
}
