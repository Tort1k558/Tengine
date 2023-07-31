#pragma once

#include<memory>

#include"Core/Math.h"

enum class TextureType
{
	R8,
	RG8,
	RGB8,
	RGBA8,
};

enum class TextureFilter
{
	Bilinear,
	Trilinear,
	Anisotropic4,
	Anisotropic8,
	Anisotropic16
};

class Texture
{
public:
	virtual ~Texture() = default;

	virtual void bind(unsigned int slot) = 0;

	UVec2 getSize();
	void* getData();
	virtual unsigned int getId() = 0;
	static std::shared_ptr<Texture> Create(void* data, UVec2 size, TextureType type);

protected:
	UVec2 m_size;
	void* m_data;
};