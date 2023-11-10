#include"RendererSystem.h"

#include"Renderer/OpenGL/RendererContextOpenGL.h"
#include"Components/Transform.h"
#include"Utils/Mesh.h"
#include"Core/Logger.h"
#include"Core/AssetManager.h"
#include"Renderer/VertexArray.h"
#include"Renderer/VertexBuffer.h"
#include"Renderer/IndexBuffer.h"
#include"Renderer/Shader.h"
#include"Scene/Scene.h"
#include"Scene/SceneManager.h"
#include"Utils/Primitives.h"
#include"Components/Model.h"
#include"Renderer/Shaders/ShaderCode.h"
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
			m_context = std::make_shared<RendererContextOpenGL>();
		}
		}
		m_context->init();
		m_context->enableDepthTest();
		ShaderSource defaultShaderSource = ShaderCode::GetDefaultShader();
		AssetManager::AddShader("DefaultShader", defaultShaderSource.getSourceShader(ShaderType::Vertex), defaultShaderSource.getSourceShader(ShaderType::Fragment));
		ShaderSource framebufferShaderSource = ShaderCode::GetFramebufferShader();
		AssetManager::AddShader("FramebufferShader", framebufferShaderSource.getSourceShader(ShaderType::Vertex), framebufferShaderSource.getSourceShader(ShaderType::Fragment));
		m_framebuffer = FrameBuffer::Create(m_viewportSize);

	}

	void RendererSystem::update()
	{
		m_framebuffer->bind();
		std::shared_ptr<Scene> scene = SceneManager::GetCurrentScene();
		if (scene)
		{
			std::shared_ptr<Camera> camera = scene->getMainCamera();
			if (!camera)
			{
				std::vector<std::shared_ptr<Camera>> cameras = scene->getComponents<Camera>();
				if (!cameras.empty())
				{
					scene->setMainCamera(cameras[0]);
					camera = cameras[0];
				}
				else
				{
					return;
				}
			}
			m_context->clearColor({ 0.1f,0.1f,0.1f, 1.0f });
			m_context->clear();
			renderCamera(camera);
			
		}
		m_framebuffer->unbind();
		if (renderToDefaultFramebuffer)
		{
			renderFramebuffer(m_framebuffer);
		}
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

	RendererType RendererSystem::getRendererType()
	{
		return m_context->getType();
	}

	TextureFilter RendererSystem::getTextureFilter()
	{
		return m_textureFilter;
	}

	std::shared_ptr<FrameBuffer> RendererSystem::getFramebuffer()
	{
		return m_framebuffer;
	}

	void RendererSystem::renderFramebuffer(std::shared_ptr<FrameBuffer> framebuffer)
	{
		FrameBuffer::SetDefaultBuffer();
		m_context->clearColor({ 1.0f,1.0f,1.0f, 1.0f });
		m_context->clear();
		std::shared_ptr<Shader> framebufferShader = AssetManager::GetResource<Shader>("FramebufferShader");
		framebufferShader->bind();
		framebuffer->getColorTexture()->bind(0);
		std::shared_ptr<Mesh> quad = Primitives::CreateQuad();
		m_context->drawIndexed(quad->getSubmeshes()[0]->getVertexArray());
	}

	void RendererSystem::disableRenderToDefaultFramebuffer()
	{
		renderToDefaultFramebuffer = false;
	}

	void RendererSystem::enableRenderToDefaultFramebuffer()
	{
		renderToDefaultFramebuffer = true;
	}

	void RendererSystem::renderCamera(std::shared_ptr<Camera> camera)
	{
		std::shared_ptr<Scene> scene = SceneManager::GetCurrentScene();
		if (scene)
		{
			std::vector<std::shared_ptr<Model>> models = scene->getComponents<Model>();
			std::shared_ptr<Shader> defaultShader = AssetManager::GetResource<Shader>("DefaultShader");
			defaultShader->bind();
			defaultShader->setUniformMat4("u_view", camera->getViewMatrix());
			defaultShader->setUniformMat4("u_projection", camera->getProjection()->getProjectionMatrix());
			for (auto& model : models)
			{
				std::shared_ptr<Transform> transform = model->getParent()->getComponent<Transform>();
				defaultShader->setUniformMat4("u_model", transform->getMatrix());
				defaultShader->setUniformVec3("u_viewPos", camera->getParent()->getComponent<Transform>()->getPosition());

				if (model->getMesh())
				{
					std::vector<std::shared_ptr<SubMesh>> submeshes = model->getMesh()->getSubmeshes();
					for (size_t i = 0; i < submeshes.size(); i++)
					{
						if (model->hasSubmeshMaterial(i))
						{
							std::shared_ptr<Material> material = model->getSubmeshMaterial(i);
							if (material->hasTexture(MaterialTexture::Diffuse))
							{
								material->getTexture(MaterialTexture::Diffuse)->bind(0);
							}
							if (material->hasTexture(MaterialTexture::Normal))
							{
								material->getTexture(MaterialTexture::Normal)->bind(1);
							}
							if (material->hasTexture(MaterialTexture::Specular))
							{
								material->getTexture(MaterialTexture::Specular)->bind(2);
							}
							if (material->hasTexture(MaterialTexture::Roughness))
							{
								material->getTexture(MaterialTexture::Roughness)->bind(3);
							}
						}
						m_context->drawIndexed(submeshes[i]->getVertexArray());
					}
				}
			}
		}
	}

	std::shared_ptr<RendererContext> RendererSystem::getRendererContext()
	{
		return m_context;
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