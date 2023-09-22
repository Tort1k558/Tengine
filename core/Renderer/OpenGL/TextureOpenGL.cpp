#include "TextureOpenGL.h"

#include"Core/Logger.h"
#include"Systems/RendererSystem.h"

namespace Tengine
{
	GLenum TextureTypeToOpenGLDataType(TextureType type)
	{
		switch (type)
		{
		case TextureType::R8:
			return GL_RED;
		case TextureType::RG8:
			return GL_RG;
		case TextureType::RGB8:
			return GL_RGB;
		case TextureType::RGBA8:
			return GL_RGBA;
		default:
			Logger::Info("ERROR::OpenGL::Unknown texture type!");
			return 0;
		}
	}

	GLenum TextureTypeToOpenGLInternalType(TextureType type)
	{
		switch (type)
		{
		case TextureType::R8:
			return GL_R8;
		case TextureType::RG8:
			return GL_RG8;
		case TextureType::RGB8:
			return GL_RGB8;
		case TextureType::RGBA8:
			return GL_RGBA8;
		case TextureType::DEPTH32:
			return GL_DEPTH_COMPONENT32;
		default:
			Logger::Info("ERROR::OpenGL::Unknown texture type!");
			return 0;
		}
	}

	TextureOpenGL::TextureOpenGL(void* data, UVec2 size, TextureType type, TextureFilter filter)
	{
		m_type = type;
		m_filter = filter;
		m_size = size;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
		GLsizei mipLevels = static_cast<GLsizei>(log2(std::max(m_size.x, m_size.y))) + 1;
		glTextureStorage2D(m_id, mipLevels, TextureTypeToOpenGLInternalType(m_type), m_size.x, m_size.y);
		if (data)
		{
			glTextureSubImage2D(m_id, 0, 0, 0, m_size.x, m_size.y, TextureTypeToOpenGLDataType(m_type), GL_UNSIGNED_BYTE, data);
		}
		if (m_filter == TextureFilter::None)
		{
			m_filter = RendererSystem::GetInstance()->getTextureFilter();
		}
		switch (m_filter)
		{
		case TextureFilter::Bilinear:
			glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			break;
		case TextureFilter::Trilinear:
			glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
		case TextureFilter::Anisotropic4:
		{
			int maxAnisotropy;
			glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			if (4 > maxAnisotropy)
			{
				Logger::Critical("Max Anisotropy {0} your is {1}", maxAnisotropy, 4);
				glTextureParameteri(m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
			}
			else
			{
				glTextureParameteri(m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
			}
			glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
		}
		case TextureFilter::Anisotropic8:
		{
			int maxAnisotropy;
			glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			if (8 > maxAnisotropy)
			{
				Logger::Critical("Max Anisotropy {0} your is {1}", maxAnisotropy, 8);
				glTextureParameteri(m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
			}
			else
			{
				glTextureParameteri(m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
			}
			glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
		}
		case TextureFilter::Anisotropic16:
		{
			int maxAnisotropy;
			glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			if (16 > maxAnisotropy)
			{
				Logger::Critical("Max Anisotropy {0} your is {1}", maxAnisotropy, 16);
				glTextureParameteri(m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
			}
			else
			{
				glTextureParameteri(m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
			}
			glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
		}
		}
		glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glGenerateTextureMipmap(m_id);
	}

	TextureOpenGL::~TextureOpenGL()
	{
		glDeleteTextures(1, &m_id);
	}

	TextureOpenGL::TextureOpenGL(TextureOpenGL&& texture) noexcept
	{
		glDeleteTextures(1, &m_id);
		m_size = texture.m_size;
		m_id = texture.m_id;
		texture.m_id = 0;
	}

	TextureOpenGL& TextureOpenGL::operator=(TextureOpenGL&& texture) noexcept
	{
		m_size = texture.m_size;
		m_id = texture.m_id;
		texture.m_id = 0;
		return *this;
	}

	void TextureOpenGL::bind(unsigned int slot)
	{
		glBindTextureUnit(slot, m_id);
	}

	void* TextureOpenGL::getData()
	{
		bind(0);
		void* pixels;
		glGetTexImage(m_id, 0, TextureTypeToOpenGLDataType(m_type), GL_UNSIGNED_BYTE, pixels);
		return pixels;
	}

	unsigned int TextureOpenGL::getId()
	{
		return m_id;
	}
}