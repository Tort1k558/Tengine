#pragma once
#include<memory>

namespace Tengine
{

	class UniformBuffer
	{
	public:

		virtual ~UniformBuffer() = default;

		virtual void setBinding(unsigned int binding) = 0;
		virtual void setData(unsigned int size, const void* data, unsigned int offset = 0) = 0;

		static std::shared_ptr<UniformBuffer> Create(unsigned int size, const void* data = nullptr, unsigned int offset = 0);
	};
}
