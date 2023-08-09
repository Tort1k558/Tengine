#include "Logger.h"

namespace Tengine
{

	void Logger::SetLevel(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::Off:
			spdlog::set_level(spdlog::level::off);
			break;
		case LogLevel::Debug:
			spdlog::set_level(spdlog::level::debug);
			break;
		case LogLevel::Info:
			spdlog::set_level(spdlog::level::info);
			break;
		case LogLevel::Critical:
			spdlog::set_level(spdlog::level::critical);
			break;
		}
	}

	void Logger::SetPattern(const std::string& pattern)
	{
		spdlog::set_pattern(pattern);
	}
}