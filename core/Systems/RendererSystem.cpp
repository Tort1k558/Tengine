#include"RendererSystem.h"

#include"Renderer/OpenGL/RendererContextOpenGL.h"
#include"Components/Transform.h"
#include"Components/Camera.h"
#include"Components/Mesh.h"
#include"Core/Logger.h"
#include"Core/Timer.h"
#include"Core/AssetManager.h"
#include"Renderer/VertexArray.h"
#include"Renderer/VertexBuffer.h"
#include"Renderer/IndexBuffer.h"
#include"Renderer/Shader.h"
#include"Scene/Scene.h"
#include"Scene/SceneManager.h"
#include"Utils/Primitives.h"

namespace Tengine
{
	std::shared_ptr<RendererSystem> RendererSystem::m_instance;

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
		AssetManager::LoadShader("data/Shaders/GLSL/screenShader.vs", "data/Shaders/GLSL/screenShader.fs");
		AssetManager::LoadTexture("data/Textures/whiteblackquads.png");
		m_framebuffer = FrameBuffer::Create(m_viewportSize);

	}

	void RendererSystem::update()
	{
		m_framebuffer->bind();
		m_context->clearColor({ 0.1f,0.1f,0.1f, 1.0f });
		m_context->clear();
		std::vector<std::shared_ptr<Camera>> cameras = SceneManager::GetCurrentScene()->getComponents<Camera>();
		for (auto& camera : cameras)
		{
			std::vector<std::shared_ptr<Mesh>> meshes = SceneManager::GetCurrentScene()->getComponents<Mesh>();
			std::shared_ptr<Shader> defaultShader = AssetManager::GetResource<Shader>("data/Shaders/GLSL/vs.vsdata/Shaders/GLSL/fs.fs");
			defaultShader->bind();
			defaultShader->setUniformMat4("u_view", camera->getViewMatrix());
			defaultShader->setUniformMat4("u_projection", camera->getProjection()->getProjectionMatrix());
			for (auto& mesh : meshes)
			{
				std::shared_ptr<Transform> transform = mesh->getParent()->getComponent<Transform>();
				defaultShader->setUniformMat4("u_model", transform->getMatrix());

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
		m_framebuffer->unbind();

		//m_context->clearColor({ 1.0f,1.0f,1.0f, 1.0f });
		//m_context->clear();
		//std::shared_ptr<Shader> screenShader = AssetManager::GetResource<Shader>("data/Shaders/GLSL/screenShader.vsdata/Shaders/GLSL/screenShader.fs");
		//screenShader->bind();
		//m_framebuffer->getColorTexture()->bind(0);
		//std::shared_ptr<Mesh> quad = Primitives::CreateQuad();
		//m_context->drawIndexed(quad->getSubmeshes()[0]->getVertexArray());

	}

	void RendererSystem::updateViewport(UVec2 size)
	{
		m_viewportSize = size;
		if (m_context)
		{
			m_context->setViewport(0, 0, m_viewportSize.x, m_viewportSize.y);
			m_framebuffer = FrameBuffer::Create(m_viewportSize);
		}
	}

	void RendererSystem::setRendererType(RendererType type)
	{
		m_rendererType = type;
	}

	void RendererSystem::setTextureFilter(TextureFilter filter)
	{
		m_textureFilter = filter;
	}

	std::shared_ptr<FrameBuffer> RendererSystem::getFramebuffer()
	{
		return m_framebuffer;
	}

	std::shared_ptr<RendererSystem> RendererSystem::GetInstance()
	{
		if (!m_instance)
		{
			m_instance = std::make_shared<RendererSystem>();
		}
		return m_instance;
	}

	void RendererSystem::destroy()
	{

	}
}