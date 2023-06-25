#pragma once
#include<vector>
#include<string>
#include<spdlog/spdlog.h>
enum class ElementType
{
	Float,
	Float2,
	Float3,
	Float4,
	Int,
	Int2,
	Int3,
	Int4,
	Bool
};
unsigned int GetCountOfElementType(ElementType type);
unsigned int ElementTypeToSize(ElementType type);
struct BufferElement
{
	BufferElement(ElementType type, bool normalized) :
		type(type), normalized(normalized), offset(0){}
	ElementType type;
	bool normalized;
	unsigned int offset;
};
class BufferLayout
{
public:
	BufferLayout() = default;
	BufferLayout(std::initializer_list<BufferElement> elements);
	unsigned int getStride() { return m_stride; }
	std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
	std::vector<BufferElement>::iterator end() { return m_elements.end(); }
private:
	std::vector<BufferElement> m_elements;
	unsigned int m_stride;
};