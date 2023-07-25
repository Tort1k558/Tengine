#include "VertexArrayOpenGL.h"

int TypeOfElementTypeToGL(ElementType type)
{
	switch (type)
	{
	case ElementType::Float:
	case ElementType::Float2:
	case ElementType::Float3:
	case ElementType::Float4:
		return GL_FLOAT;
	case ElementType::Int:
	case ElementType::Int2:
	case ElementType::Int3:
	case ElementType::Int4:
		return GL_INT;
	case ElementType::Bool:
		return GL_BOOL;
	default:
		break;
	}
	return 0;
}

VertexArrayOpenGL::VertexArrayOpenGL() : 
	m_elementCount(0)
{
	glCreateVertexArrays(1, &m_id);
}

VertexArrayOpenGL::~VertexArrayOpenGL()
{
	glDeleteVertexArrays(1, &m_id);
}

VertexArrayOpenGL::VertexArrayOpenGL(VertexArrayOpenGL&& array) noexcept :
	m_id(array.m_id),m_elementCount(array.m_elementCount)
{
	array.m_id = 0;
	array.m_elementCount = 0;
}

VertexArrayOpenGL& VertexArrayOpenGL::operator=(VertexArrayOpenGL&& array) noexcept
{
	m_id = array.m_id;
	m_elementCount = array.m_elementCount;
	array.m_id = 0;
	array.m_elementCount = 0;
	return *this;
}

void VertexArrayOpenGL::bind()
{
	glBindVertexArray(m_id);
}

void VertexArrayOpenGL::unbind()
{
	glBindVertexArray(0);
}

void VertexArrayOpenGL::addVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
{
	bind();
	m_vertexBuffers.push_back(vertexBuffer);
	vertexBuffer->bind();
	BufferLayout layout = vertexBuffer->getLayout();
	for (auto& element : layout)
	{
		glEnableVertexAttribArray(m_elementCount);
		glVertexAttribPointer(m_elementCount, GetCountOfElementType(element.type), TypeOfElementTypeToGL(element.type), element.normalized, layout.getStride(), (const void*)(element.offset));
		m_elementCount++;
	}
}

void VertexArrayOpenGL::setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
{
	bind();

	m_indexBuffer = indexBuffer;
	m_indexBuffer->bind();
}

unsigned int VertexArrayOpenGL::getCountOfIndices()
{
	if (m_indexBuffer)
	{
		return m_indexBuffer->getCountOfIndices();
	}
	return 0;
}
