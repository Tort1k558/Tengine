#pragma once

#include<memory>

#include"Core/Math.h"

enum class TextureType
{
	RGB8,
	RGBA8,
};

class Texture
{
public:
	virtual ~Texture() = default;

	virtual void bind(unsigned int slot) = 0;

	UVec2 getSize() { return m_size; }

	static std::shared_ptr<Texture> Create(void* data, UVec2 size, TextureType type);

protected:
	UVec2 m_size;
};