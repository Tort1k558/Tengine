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
std::shared_ptr<VertexBuffer> vbPosCol;
std::shared_ptr<IndexBuffer> ibPosCol;

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
	vbPosCol = VertexBuffer::Create(posCol, sizeof(posCol), BufferUsage::Static);
	BufferLayout posCol;
	posCol.push({ ElementType::Float3 });
	posCol.push({ ElementType::Float2 });

	vbPosCol->setLayout(posCol);
	va->addVertexBuffer(vbPosCol);
	ibPosCol = IndexBuffer::Create(indices, 6);
	va->setIndexBuffer(ibPosCol);

	texture = AssetManager::LoadTexture("awesomeFace","data/Textures/awesomeface.png");
}

void RendererSystem::update()
{
	m_context->clearColor({ 0.0f,0.0f,0.0f, 1.0f });
	m_context->clear();
	static double delta = 0.0f;
	delta += Timer::GetDeltaTime() * 100;
	std::shared_ptr<Transform> transform = SceneManager::GetCurrentScene()->getComponents<Transform>()[0];
	std::shared_ptr<Camera> camera = SceneManager::GetCurrentScene()->getComponents<Camera>()[0];
	//transform->setRotation({ delta, delta, delta });
	shader->setUniformMat4("u_modelMatrix", transform->getMatrix());
	shader->setUniformMat4("u_viewMatrix", camera->getViewMatrix());
	shader->setUniformMat4("u_projectionMatrix", camera->getProjectionMatrix());
	texture->bind(0);
	shader->bind();
	va->bind();
	m_context->drawIndexed(va);

}

void RendererSystem::updateWindowSize(unsigned int widht, unsigned int height)
{
	m_context->setViewport(0, 0, widht, height);
}

void RendererSystem::setRendererType(RendererType type)
{
	m_rendererType = type;
}

void RendererSystem::destroy()
{

}

RendererType RendererSystem::getType()
{
	return m_context->getType();
}
