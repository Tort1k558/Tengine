#include "RendererContextOpenGL.h"

#include"Core/Logger.h"

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

RendererType RendererContextOpenGL::getType()
{
	return RendererType::OpenGL;
}
