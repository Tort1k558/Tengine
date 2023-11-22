#include"CubeMapTextureOpenGL.h"

#include"Renderer/OpenGL/TextureOpenGL.h"

namespace Tengine
{
	CubeMapTextureOpenGL::CubeMapTextureOpenGL(std::array<std::shared_ptr<Image>, 6> images, std::array<TextureType, 6> types, TextureFilter filter) :
		m_id(0)
	{
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

		for (size_t i = 0; i < images.size(); i++)
		{
			if (images[i]->getData())
			{
				UVec2 size = images[i]->getSize();
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, TextureTypeToOpenGLInternalFormat(types[i]), size.x, size.y,
					0, TextureTypeToOpenGLFormat(types[i]), TextureTypeToOpenGLType(types[i]), images[i]->getData());
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	CubeMapTextureOpenGL::~CubeMapTextureOpenGL()
	{
		glDeleteTextures(1, &m_id);
	}

	CubeMapTextureOpenGL::CubeMapTextureOpenGL(CubeMapTextureOpenGL&& texture) noexcept
	{
		m_id = texture.m_id;
		texture.m_id = 0;
	}

	CubeMapTextureOpenGL& CubeMapTextureOpenGL::operator=(CubeMapTextureOpenGL&& texture) noexcept
	{
		m_id = texture.m_id;
		texture.m_id = 0;
		return *this;
	}

	void CubeMapTextureOpenGL::bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
	}
	unsigned int CubeMapTextureOpenGL::getId()
	{
		return m_id;
	}
}