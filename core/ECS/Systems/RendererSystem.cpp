#include "RendererSystem.h"

#include "Renderer/OpenGL/RendererContextOpenGL.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/Mesh.h"
#include "Core/Logger.h"
#include "Core/Timer.h"
#include "Core/AssetManager.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "ECS/ComponentManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"


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
	AssetManager::LoadShader("data/Shaders/GLSL/vs.vs", "data/Shaders/GLSL/fs.fs");
	AssetManager::LoadTexture("data/Textures/whiteblackquads.png");

}

void RendererSystem::update()
{
	m_context->clearColor({ 0.1f,0.1f,0.1f, 1.0f });
	m_context->clear();
	std::vector<std::shared_ptr<Camera>> cameras = SceneManager::GetCurrentScene()->getComponents<Camera>();
	for (auto& camera : cameras)
	{
		std::vector<std::shared_ptr<Mesh>> meshes = SceneManager::GetCurrentScene()->getComponents<Mesh>();
		std::shared_ptr<Shader> shader = AssetManager::GetResource<Shader>("data/Shaders/GLSL/vs.vsdata/Shaders/GLSL/fs.fs");
		shader->bind();
		shader->setUniformMat4("u_view", camera->getViewMatrix());
		shader->setUniformMat4("u_projection", camera->getProjectionMatrix());
		for (auto& mesh : meshes)
		{
			std::shared_ptr<Transform> transform = mesh->getParent()->getComponent<Transform>();
			shader->setUniformMat4("u_model", transform->getMatrix());
			
			std::vector<std::shared_ptr<SubMesh>> submeshes = mesh->getSubmeshes();
			for (auto& submesh : submeshes)
			{
				if (submesh->hasMaterial())
				{
					std::shared_ptr<Material> material = submesh->getMaterial();
					if (material->hasTexture(MaterialTexture::Diffuse))
					{
						material->getTexture(MaterialTexture::Diffuse)->bind(0);
					}
				}
				m_context->drawIndexed(submesh->getVertexArray());
			}
		}
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
