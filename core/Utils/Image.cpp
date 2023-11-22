#include "Image.h"

namespace Tengine
{
	Image::Image(void* data, UVec2 size, int channels) :
		m_data(data), m_size(size),m_channels(channels)
	{
		
	}
	Image::~Image()
	{
		if (m_data)
		{
			free(m_data);
		}
	}

	void* Image::getData()
	{
		return m_data;
	}

	UVec2 Image::getSize()
	{
		return m_size;
	}

	int Image::getChannels()
	{
		return m_channels;
	}
}