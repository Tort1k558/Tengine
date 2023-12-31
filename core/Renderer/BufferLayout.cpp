#include "BufferLayout.h"
#include "Core/Logger.h"

namespace Tengine
{
	BufferLayout::BufferLayout() :
		m_elements({}), m_stride(0)
	{

	}

	BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements) :
		m_elements(elements), m_stride(0)
	{
		calculateStride();
	}

	void BufferLayout::push(BufferElement element)
	{
		m_elements.push_back(element);
		calculateStride();
	}

	void BufferLayout::push(std::initializer_list<BufferElement> elements)
	{
		m_elements.insert(m_elements.end(), elements);
		calculateStride();
	}

	void BufferLayout::calculateStride()
	{
		unsigned int offset = 0;
		m_stride = 0;
		for (auto& element : m_elements)
		{
			element.offset = offset;

			unsigned int size = ElementTypeToSize(element.type);
			m_stride += size;
			offset += size;
		}
	}

	unsigned int GetCountOfElementType(ElementType type)
	{
		switch (type)
		{
		case ElementType::Float:
		case ElementType::Int:
		case ElementType::Bool:
			return 1;
		case ElementType::Float2:
		case ElementType::Int2:
			return 2;
		case ElementType::Float3:
		case ElementType::Int3:
			return 3;
		case ElementType::Float4:
		case ElementType::Int4:
			return 4;
		default:
			break;
		}
		return 0;
	}

	unsigned int ElementTypeToSize(ElementType type)
	{
		switch (type)
		{
		case ElementType::Float:
			return 4;
		case ElementType::Float2:
			return 4 * 2;
		case ElementType::Float3:
			return 4 * 3;
		case ElementType::Float4:
			return 4 * 4;
		case ElementType::Int:
			return 4;
		case ElementType::Int2:
			return 4 * 2;
		case ElementType::Int3:
			return 4 * 3;
		case ElementType::Int4:
			return 4 * 4;
		case ElementType::Bool:
			return 1;
		default:
			Logger::Critical("ERROR::Unknown element type");
			break;
		}
		return 0;
	}
}