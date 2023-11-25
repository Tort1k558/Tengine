#pragma once

#include"Renderer/CubeMapTexture.h"
#include"Renderer/OpenGL/RendererContextOpenGL.h"

namespace Tengine
{
	class CubeMapTextureOpenGL : public CubeMapTexture
	{
	public:
		CubeMapTextureOpenGL(std::array<std::shared_ptr<Texture>, 6> textures, TextureFilter filter = TextureFilter::None);
		~CubeMapTextureOpenGL();
		CubeMapTextureOpenGL(const CubeMapTextureOpenGL&) = delete;
		CubeMapTextureOpenGL& operator=(const CubeMapTextureOpenGL&) = delete;
		CubeMapTextureOpenGL(CubeMapTextureOpenGL&& texture) noexcept;
		CubeMapTextureOpenGL& operator=(CubeMapTextureOpenGL&& texture) noexcept;

		void bind(unsigned int slot) final;
		unsigned int getId() final;
		virtual void setTexture(CubeMapSide side, std::shared_ptr<Texture> texture) final;
		virtual std::shared_ptr<void> getData(CubeMapSide side) final;
	private:
		GLuint m_id;
	};
}