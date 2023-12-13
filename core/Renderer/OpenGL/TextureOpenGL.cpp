#include "TextureOpenGL.h"

#include"Core/Logger.h"
#include"Systems/RendererSystem.h"

namespace Tengine
{
	GLenum TextureTypeToOpenGLFormat(TextureType type)
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
		case TextureType::DEPTH32F:
			return GL_DEPTH_COMPONENT;
		default:
			Logger::Info("ERROR::OpenGL::Unknown texture type!");
			return 0;
		}
	}

	GLenum TextureTypeToOpenGLInternalFormat(TextureType type)
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
		case TextureType::DEPTH32F:
			return GL_DEPTH_COMPONENT32F;
		default:
			Logger::Info("ERROR::OpenGL::Unknown texture type!");
			return 0;
		}
	}

	TextureType OpenGLInternalFormatToTextureType(GLint type)
	{
		switch (type)
		{
		case GL_R8:
			return TextureType::R8;
		case GL_RG8:
			return TextureType::RG8;
		case GL_RGB8:
			return TextureType::RGB8;
		case GL_RGBA8:
			return TextureType::RGBA8;
		case GL_DEPTH_COMPONENT32F:
			return TextureType::DEPTH32F;
		default:
			Logger::Info("ERROR::OpenGL::Unknown texture type!");
			return TextureType::RGB8;
		}
	}

	GLenum TextureTypeToOpenGLType(TextureType type)
	{
		switch (type)
		{
		case TextureType::R8:
			return GL_UNSIGNED_BYTE;
		case TextureType::RG8:
			return GL_UNSIGNED_BYTE;
		case TextureType::RGB8:
			return GL_UNSIGNED_BYTE;
		case TextureType::RGBA8:
			return GL_UNSIGNED_BYTE;
		case TextureType::DEPTH32F:
			return GL_FLOAT;
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

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexImage2D(GL_TEXTURE_2D, 0, TextureTypeToOpenGLInternalFormat(m_type), m_size.x, m_size.y, 0,
			TextureTypeToOpenGLFormat(m_type), TextureTypeToOpenGLType(m_type), data ? data : NULL);
		glGenerateMipmap(GL_TEXTURE_2D);
		if (m_filter == TextureFilter::None)
		{
			m_filter = RendererSystem::GetInstance()->getTextureFilter();
		}
		switch (m_filter)
		{
		case TextureFilter::None:
		case TextureFilter::Bilinear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			break;
		case TextureFilter::Trilinear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
		case TextureFilter::Anisotropic4:
		{
			int maxAnisotropy;
			glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			if (4 > maxAnisotropy)
			{
				Logger::Critical("Max Anisotropy {0} your is {1}", maxAnisotropy, 4);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
		}
		case TextureFilter::Anisotropic8:
		{
			int maxAnisotropy;
			glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			if (8 > maxAnisotropy)
			{
				Logger::Critical("Max Anisotropy {0} your is {1}", maxAnisotropy, 8);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
		}
		case TextureFilter::Anisotropic16:
		{
			int maxAnisotropy;
			glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			if (16 > maxAnisotropy)
			{
				Logger::Critical("Max Anisotropy {0} your is {1}", maxAnisotropy, 16);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
		}
		default:
			break;
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	TextureOpenGL::~TextureOpenGL()
	{
		glDeleteTextures(1, &m_id);
	}

	TextureOpenGL::TextureOpenGL(TextureOpenGL&& texture) noexcept
	{
		m_id = texture.m_id;
		texture.m_id = 0;
	}

	TextureOpenGL& TextureOpenGL::operator=(TextureOpenGL&& texture) noexcept
	{
		m_id = texture.m_id;
		texture.m_id = 0;
		return *this;
	}

	void TextureOpenGL::bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	std::shared_ptr<void> TextureOpenGL::getData()
	{
		std::shared_ptr<void> pixels(malloc(m_size.x * m_size.y * TextureTypeToSize(m_type)), [](void* ptr) { free(ptr); });
		bind(0);
		glGetTexImage(GL_TEXTURE_2D, 0, TextureTypeToOpenGLFormat(m_type), GL_UNSIGNED_BYTE, pixels.get());
		return pixels;
	}

	unsigned int TextureOpenGL::getId()
	{
		return m_id;
	}


	MultisampleTextureOpenGL::MultisampleTextureOpenGL(void* data, UVec2 size, TextureType type, size_t samples)
	{
		m_type = type;
		m_size = size;
		m_samples = samples;

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_id);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, TextureTypeToOpenGLInternalFormat(m_type), m_size.x, m_size.y, GL_TRUE);
	}

	MultisampleTextureOpenGL::MultisampleTextureOpenGL(MultisampleTextureOpenGL&& texture) noexcept
	{
		m_id = texture.m_id;
		texture.m_id = 0;
	}

	MultisampleTextureOpenGL& MultisampleTextureOpenGL::operator=(MultisampleTextureOpenGL&& texture) noexcept
	{
		m_id = texture.m_id;
		texture.m_id = 0;
		return *this;
	}

	MultisampleTextureOpenGL::~MultisampleTextureOpenGL()
	{
		glDeleteTextures(1, &m_id);
	}

	void MultisampleTextureOpenGL::bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_id);
	}

	unsigned int MultisampleTextureOpenGL::getId()
	{
		return m_id;
	}
}