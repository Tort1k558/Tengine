#pragma once
#include<spdlog/spdlog.h>
#include<string>

class Logger
{
public:
	Logger() = delete;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	template<typename... Args>
	static void Info(const std::string& format, Args... args)
	{
		spdlog::info(format, args...);
	}

	template<typename... Args>
	static void Debug(const std::string& format, Args... args)
	{
		spdlog::debug(format, args...);
	}

	template<typename... Args>
	static void Critical(const std::string& format, Args... args)
	{
		spdlog::critical(format, args...);
	}
};