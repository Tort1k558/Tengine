#include "FrameBufferOpenGL.h"

#include"Core/Logger.h"

namespace Tengine
{
	FrameBufferOpenGL::FrameBufferOpenGL()
	{
		glGenFramebuffers(1, &m_id);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
		{
			Logger::Critical("ERROR::OpenGL::FrameBuffer::Failed to create!");
			return;
		}
	}

	FrameBufferOpenGL::~FrameBufferOpenGL()
	{
		glDeleteFramebuffers(1, &m_id);
	}

	FrameBufferOpenGL::FrameBufferOpenGL(FrameBufferOpenGL&& buffer) noexcept
	{
		m_id = buffer.m_id;
		m_attachments = std::move(buffer.m_attachments);
		m_multisampledTexture = std::move(buffer.m_multisampledTexture);

		buffer.m_id = 0;
	}

	FrameBufferOpenGL& FrameBufferOpenGL::operator=(FrameBufferOpenGL&& buffer) noexcept
	{
		m_id = buffer.m_id;
		m_attachments = std::move(buffer.m_attachments);
		m_multisampledTexture = std::move(buffer.m_multisampledTexture);

		buffer.m_id = 0;

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

	void FrameBufferOpenGL::attachTexture(std::shared_ptr<Texture> texture, FrameBufferAttachment attachment)
	{
		m_attachments[attachment] = texture;
		bind();
		switch (attachment)
		{
		case Tengine::FrameBufferAttachment::Color:
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getId(), 0);
			break;
		case Tengine::FrameBufferAttachment::Depth:
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getId(), 0);
			break;
		case Tengine::FrameBufferAttachment::Stencil:
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture->getId(), 0);
			break;
		default:
			break;
		}
		unbind();
	}

	void FrameBufferOpenGL::attachColorMultisampleTexture(std::shared_ptr<MultisampleTexture> texture)
	{
		m_multisampledTexture = texture;
		m_attachments[FrameBufferAttachment::Color] = nullptr;
		bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_multisampledTexture->getId(), 0);
		unbind();
	}

	std::shared_ptr<Texture> FrameBufferOpenGL::getAttachment(FrameBufferAttachment attachment)
	{
		if (m_attachments.find(attachment) == m_attachments.end())
		{
			return nullptr;
		}
		return m_attachments[attachment];
	}

	void FrameBufferOpenGL::copy(std::shared_ptr<FrameBuffer> buffer, UVec2 src0, UVec2 src1, UVec2 dst0, UVec2 dst1, FrameBufferAttachment mask, FrameBufferCopyFilter filter)
	{
		buffer->bindToRead();
		bindToWrite();
		GLbitfield maskOpenGL = 0;
		switch (mask)
		{
		case Tengine::FrameBufferAttachment::Color:
			maskOpenGL = GL_COLOR_BUFFER_BIT;
			break;
		case Tengine::FrameBufferAttachment::Depth:
			maskOpenGL = GL_DEPTH_BUFFER_BIT;
			break;
		case Tengine::FrameBufferAttachment::Stencil:
			maskOpenGL = GL_STENCIL_BUFFER_BIT;
			break;
		default:
			break;
		}
		GLenum filterOpenGL = 0;
		switch (filter)
		{
		case Tengine::FrameBufferCopyFilter::Linear:
			filterOpenGL = GL_LINEAR;
			break;
		case Tengine::FrameBufferCopyFilter::Nearest:
			filterOpenGL = GL_NEAREST;
			break;
		default:
			break;
		}

		glBlitFramebuffer(src0.x, src0.y, src1.x, src1.y, dst0.x, dst0.y, dst1.x, dst1.y, maskOpenGL, filterOpenGL);
	}

	void FrameBufferOpenGL::SetDefaultBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
