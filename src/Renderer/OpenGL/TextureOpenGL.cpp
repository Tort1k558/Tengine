#include "TextureOpenGL.h"

#include"Core/Logger.h"
#include"Systems/RendererSystem.h"

GLenum TextureTypeToOpenGLDataType(TextureType type)
{
	switch (type)
	{
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
	case TextureType::RGB8:
		return GL_RGB8;
	case TextureType::RGBA8:
		return GL_RGBA8;
	default:
		Logger::Info("ERROR::OpenGL::Unknown texture type!");
		return 0;
	}
}

TextureOpenGL::TextureOpenGL(void* data, UVec2 size, TextureType type)
{
	GLsizei mipLevels = static_cast<GLsizei>(log2(std::max(size.x, size.y))) + 1;
	glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
	glTextureStorage2D(m_id, mipLevels, TextureTypeToOpenGLInternalType(type), size.x, size.y);
	switch (System::GetInstance<RendererSystem>()->getTextureFilter())
	{
	case TextureFilter::Bilinear:
		glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case TextureFilter::Trilinear:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	}

	glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureSubImage2D(m_id, 0, 0, 0, size.x, size.y, TextureTypeToOpenGLDataType(type), GL_UNSIGNED_BYTE, data);
	glGenerateTextureMipmap(m_id);
}

TextureOpenGL::~TextureOpenGL()
{
	glDeleteTextures(1, &m_id);
}

void TextureOpenGL::bind(unsigned int slot)
{
	glBindTextureUnit(slot, m_id);
}
