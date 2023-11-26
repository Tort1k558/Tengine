#include"CubeMapTextureOpenGL.h"

#include"Renderer/OpenGL/TextureOpenGL.h"



namespace Tengine
{
	void FlipImageY(void* data, UVec2 size, int bytesPerPixel)
	{
		unsigned char* bytes = static_cast<unsigned char*>(data);
		size_t bytesPerRow = static_cast<size_t>(size.x) * bytesPerPixel;

		for (int row = 0; row < size.y / 2; ++row) {
			unsigned char* rowStart = bytes + row * bytesPerRow;
			unsigned char* rowEnd = bytes + (size.y - row - 1) * bytesPerRow;

			std::vector<unsigned char> temp(rowStart, rowStart + bytesPerRow);
			std::copy(rowEnd, rowEnd + bytesPerRow, rowStart);
			std::copy(temp.begin(), temp.end(), rowEnd);
		}
	}

	CubeMapTextureOpenGL::CubeMapTextureOpenGL(std::array<std::shared_ptr<Texture>, 6> textures, TextureFilter filter) :
		m_id(0)
	{
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

		for (size_t i = 0; i < textures.size(); i++)
		{
			if (textures[i]->getData())
			{
				UVec2 size = textures[i]->getSize();
				TextureType type = textures[i]->getType();
				std::shared_ptr<void> data = textures[i]->getData();

				//We flip the texture because the origin of coordinates in CubeMap is different 
				FlipImageY(data.get(), size, TextureTypeToSize(type));
				
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, TextureTypeToOpenGLInternalFormat(type), size.x, size.y,
					0, TextureTypeToOpenGLFormat(type), TextureTypeToOpenGLType(type), data.get());
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
	void CubeMapTextureOpenGL::setSideTexture(CubeMapSide side, std::shared_ptr<Texture> texture)
	{
		UVec2 size = texture->getSize();
		TextureType type = texture->getType();
		std::shared_ptr<void> data = texture->getData();

		FlipImageY(data.get(), size, TextureTypeToSize(type));

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<int>(side), 0, TextureTypeToOpenGLInternalFormat(type), size.x, size.y,
			0, TextureTypeToOpenGLFormat(type), TextureTypeToOpenGLType(type), data.get());
	}

	std::shared_ptr<void> CubeMapTextureOpenGL::getData(CubeMapSide side)
	{
		bind(0);
		GLint width, height, internalFormat;

		glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<int>(side), 0, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<int>(side), 0, GL_TEXTURE_HEIGHT, &height);
		glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<int>(side), 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
		TextureType type = OpenGLInternalFormatToTextureType(internalFormat);
		
		std::shared_ptr<void> pixels(malloc(width * height * TextureTypeToSize(type)), [](void* ptr) { free(ptr); });
		glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<int>(side), 0, TextureTypeToOpenGLFormat(type), GL_UNSIGNED_BYTE, pixels.get());
		return pixels;
	}
}