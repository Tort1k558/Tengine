#pragma once

#include"Renderer/CubeMapTexture.h"
#include"Renderer/OpenGL/RendererContextOpenGL.h"

namespace Tengine
{
	class CubeMapTextureOpenGL : public CubeMapTexture
	{
	public:
		CubeMapTextureOpenGL(std::array<std::shared_ptr<Image>, 6> images, std::array<TextureType, 6> types, TextureFilter filter = TextureFilter::None);
		~CubeMapTextureOpenGL();
		CubeMapTextureOpenGL(const CubeMapTextureOpenGL&) = delete;
		CubeMapTextureOpenGL& operator=(const CubeMapTextureOpenGL&) = delete;
		CubeMapTextureOpenGL(CubeMapTextureOpenGL&& texture) noexcept;
		CubeMapTextureOpenGL& operator=(CubeMapTextureOpenGL&& texture) noexcept;

		void bind(unsigned int slot) final;
		unsigned int getId() final;
	private:
		GLuint m_id;
	};
}