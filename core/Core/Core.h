#pragma once

#ifdef _WIN32
	#define TENGINE __declspec(dllexport)
#else
	#define TENGINE
#endif