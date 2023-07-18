#include "RendererSystem.h"

#include "Renderer/OpenGL/RendererContextOpenGL.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Core/Logger.h"
#include "Core/Timer.h"
#include "Core/AssetManager.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "ECS/ComponentManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

GLfloat posCol[] =
{
	//         Positions              uv
		-0.5f, -0.5f, 0.0f,      0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,      1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,      0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,      1.0f, 0.0f
};
GLuint indices[] =
{
	0,1,2,
	0,1,3
};

std::shared_ptr<Shader> shader;
std::shared_ptr<Texture> texture;
std::shared_ptr<VertexArray> va;
std::shared_ptr<VertexBuffer> vbPosUV;
std::shared_ptr<IndexBuffer> ibPosUV;

void RendererSystem::init()
{
	switch (m_rendererType)
	{
	case RendererType::None:
		Logger::Critical("ERROR::Renderer type not specified!");
		return;
	case RendererType::OpenGL:
		{
			m_context = std::make_unique<RendererContextOpenGL>();
		}
	}
	m_context->init();
	shader = AssetManager::LoadShader("DefaultShader", "data/Shaders/GLSL/vs.vs", "data/Shaders/GLSL/fs.fs");

	va = VertexArray::Create();
	vbPosUV = VertexBuffer::Create(posCol, sizeof(posCol), BufferUsage::Static);
	BufferLayout posCol;
	posCol.push({ ElementType::Float3 });
	posCol.push({ ElementType::Float2 });

	vbPosUV->setLayout(posCol);
	va->addVertexBuffer(vbPosUV);
	ibPosUV = IndexBuffer::Create(indices, 6);
	va->setIndexBuffer(ibPosUV);

	texture = AssetManager::LoadTexture("awesomeFace","data/Textures/whiteblackquads.png");
}

void RendererSystem::update()
{
	m_context->clearColor({ 0.1f,0.1f,0.1f, 1.0f });
	m_context->clear();

	std::shared_ptr<Transform> transform = SceneManager::GetCurrentScene()->getComponents<Transform>()[0];
	std::shared_ptr<Camera> camera = SceneManager::GetCurrentScene()->getComponents<Camera>()[0];
	shader->bind();
	shader->setUniformMat4("u_modelMatrix", transform->getMatrix());
	shader->setUniformMat4("u_projectionMatrix", camera->getProjectionMatrix());
	shader->setUniformMat4("u_viewMatrix", camera->getViewMatrix());
	texture->bind(0);
	m_context->drawIndexed(va);

}

void RendererSystem::updateViewport(UVec2 size)
{
	m_viewportSize = size;
	m_context->setViewport(0, 0, m_viewportSize.x, m_viewportSize.y);
}

void RendererSystem::setRendererType(RendererType type)
{
	m_rendererType = type;
}

void RendererSystem::setTextureFilter(TextureFilter filter)
{
	m_textureFilter = filter;
}

void RendererSystem::destroy()
{

}
