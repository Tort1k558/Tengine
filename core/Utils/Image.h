#pragma once

#include"Core/AssetManager.h"
#include"Core/Math.h"

namespace Tengine
{
	class TENGINE Image : public Resource
	{
	public:
		Image(void* data, UVec2 size, int channels = 3);
		~Image();
		void* getData();
		UVec2 getSize();
		int getChannels();
		
	private:
		void* m_data;
		UVec2 m_size;
		int m_channels;
	};
}