#include "FrameBufferOpenGL.h"

#include"Core/Logger.h"

namespace Tengine
{
	FrameBufferOpenGL::FrameBufferOpenGL(UVec2 size)
	{
		m_size = size;

		glGenFramebuffers(1, &m_id);
		bind();
		std::shared_ptr<Image> image = std::make_shared<Image>(nullptr, size);
		m_colorTexture = std::dynamic_pointer_cast<TextureOpenGL>(Texture::Create(image, TextureType::RGBA8, TextureFilter::None));
		m_depthTexture = std::dynamic_pointer_cast<TextureOpenGL>(Texture::Create(image, TextureType::DEPTH32F, TextureFilter::None));

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture->m_id, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->m_id, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
		{
			Logger::Critical("ERROR::OpenGL::FrameBuffer::Failed to create!");
			return;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	void FrameBufferOpenGL::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	std::shared_ptr<Texture> FrameBufferOpenGL::getColorTexture()
	{
		return m_colorTexture;
	}
	std::shared_ptr<Texture> FrameBufferOpenGL::getDepthTexture()
	{
		return m_depthTexture;
	}
	void FrameBufferOpenGL::SetDefaultBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
