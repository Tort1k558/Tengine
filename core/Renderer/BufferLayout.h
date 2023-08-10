#pragma once
#include<vector>
#include<string>


namespace Tengine
{
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
		BufferElement(ElementType type, bool normalized = false) :
			type(type), normalized(normalized), offset(0) {}
		ElementType type;
		bool normalized;
		unsigned int offset;
	};
	class BufferLayout
	{
	public:
		BufferLayout();
		BufferLayout(std::initializer_list<BufferElement> elements);
		unsigned int getStride() { return m_stride; }
		void push(BufferElement element);
		void push(std::initializer_list<BufferElement> elements);
		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
	private:
		void calculateStride();
		std::vector<BufferElement> m_elements;
		unsigned int m_stride;
	};
}