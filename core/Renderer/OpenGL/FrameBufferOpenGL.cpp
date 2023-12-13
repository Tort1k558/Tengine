#include "FrameBufferOpenGL.h"

#include"Core/Logger.h"

namespace Tengine
{
	FrameBufferOpenGL::FrameBufferOpenGL(UVec2 size)
	{
		m_size = size;

		glGenFramebuffers(1, &m_id);
		bind();
		m_colorTexture = std::dynamic_pointer_cast<TextureOpenGL>(Texture::Create(nullptr, size, TextureType::RGBA8, TextureFilter::None));
		m_depthTexture = std::dynamic_pointer_cast<TextureOpenGL>(Texture::Create(nullptr, size, TextureType::DEPTH32F, TextureFilter::None));
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture->m_id, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->m_id, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
		{
			Logger::Critical("ERROR::OpenGL::FrameBuffer::Failed to create!");
			return;
		}
		unbind();
	}

	FrameBufferOpenGL::~FrameBufferOpenGL()
	{
		glDeleteFramebuffers(1, &m_id);
	}

	FrameBufferOpenGL::FrameBufferOpenGL(FrameBufferOpenGL&& buffer) noexcept
	{
		m_id = buffer.m_id;
		m_colorTexture = buffer.m_colorTexture;
		m_depthTexture = buffer.m_depthTexture;

		buffer.m_id = 0;
		buffer.m_colorTexture = nullptr;
		buffer.m_depthTexture = nullptr;
	}

	FrameBufferOpenGL& FrameBufferOpenGL::operator=(FrameBufferOpenGL&& buffer) noexcept
	{
		m_id = buffer.m_id;
		m_colorTexture = buffer.m_colorTexture;
		m_depthTexture = buffer.m_depthTexture;

		buffer.m_id = 0;
		buffer.m_colorTexture = nullptr;
		buffer.m_depthTexture = nullptr;

		return *this;
	}

	void FrameBufferOpenGL::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}

	void FrameBufferOpenGL::bindToRead()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
	}

	void FrameBufferOpenGL::bindToWrite()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);
	}

	void FrameBufferOpenGL::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBufferOpenGL::attachColorTexture(std::shared_ptr<Texture> texture)
	{
		m_colorTexture = texture;
		bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture->getId(), 0);
		unbind();
	}

	void FrameBufferOpenGL::attachColorMultisampleTexture(std::shared_ptr<MultisampleTexture> texture)
	{
		m_multisampledTexture = texture;
		m_colorTexture = nullptr;
		bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_multisampledTexture->getId(), 0);
		unbind();
	}

	void FrameBufferOpenGL::attachDepthTexture(std::shared_ptr<Texture> texture)
	{
		m_depthTexture = texture;
		bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->getId(), 0);
		unbind();
	}

	void FrameBufferOpenGL::attachStencilTexture(std::shared_ptr<Texture> texture)
	{
		m_stencilTexture = texture;
		bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_stencilTexture->getId(), 0);
		unbind();
	}

	std::shared_ptr<Texture> FrameBufferOpenGL::getColorTexture()
	{
		return m_colorTexture;
	}
	std::shared_ptr<Texture> FrameBufferOpenGL::getDepthTexture()
	{
		return m_depthTexture;
	}
	std::shared_ptr<Texture> FrameBufferOpenGL::getStencilTexture()
	{
		return std::shared_ptr<Texture>();
	}
	void FrameBufferOpenGL::SetDefaultBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
