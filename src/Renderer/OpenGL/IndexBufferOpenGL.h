#pragma once

#include<memory>
#include"../IndexBuffer.h"
class IndexBufferOpenGL : public IndexBuffer
{
public:
	IndexBufferOpenGL(unsigned int* indices, unsigned int size);
	void bind() override;
	void unbind() override;
};