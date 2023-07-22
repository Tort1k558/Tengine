#include "RendererSystem.h"

#include "Renderer/OpenGL/RendererContextOpenGL.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Components/Mesh.h"
#include "Core/Logger.h"
#include "Core/Timer.h"
#include "Core/AssetManager.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "ECS/ComponentManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

std::shared_ptr<Shader> shader;
std::shared_ptr<Texture> texture;

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
	m_context->enableDepthTest();
	shader = AssetManager::LoadShader("DefaultShader", "data/Shaders/GLSL/vs.vs", "data/Shaders/GLSL/fs.fs");
	texture = AssetManager::LoadTexture("awesomeFace","data/Textures/whiteblackquads.png");

}

void RendererSystem::update()
{
	m_context->clearColor({ 0.1f,0.1f,0.1f, 1.0f });
	m_context->clear();
	std::shared_ptr<Camera> camera = SceneManager::GetCurrentScene()->getComponents<Camera>()[0];
	std::vector<std::shared_ptr<Mesh>> meshes = SceneManager::GetCurrentScene()->getComponents<Mesh>();

	camera->getPerspectiveProjection()->setAspectRatio(static_cast<float>(m_viewportSize.x) / static_cast<float>(m_viewportSize.y));
	shader->bind();
	shader->setUniformMat4("u_viewMatrix", camera->getViewMatrix());
	shader->setUniformMat4("u_projectionMatrix", camera->getProjectionMatrix());
	for (auto& mesh : meshes)
	{
		std::shared_ptr<Transform> transform = mesh->getParent()->getComponent<Transform>();
		shader->setUniformMat4("u_modelMatrix", transform->getMatrix());
		texture->bind(0);
		m_context->drawIndexed(mesh->getVertexArray());
	}
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
