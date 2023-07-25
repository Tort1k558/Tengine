#pragma once
#include<spdlog/spdlog.h>
#include<string>

enum class LogLevel
{
	Off,
	Debug,
	Info,
	Critical
};

class Logger
{
public:
	Logger() = delete;
	Logger(const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger& operator=(const Logger&) = delete;
	Logger& operator=(Logger&&) = delete;

	template<typename... Args>
	static void Info(const std::string& format, Args... args);
	template<typename... Args>
	static void Debug(const std::string& format, Args... args);
	template<typename... Args>
	static void Critical(const std::string& format, Args... args);
	template<typename... Args>
	static void Warning(const std::string& format, Args... args);

	static void SetLevel(LogLevel level);
	static void SetPattern(const std::string& pattern);
};

template<typename ...Args>
inline void Logger::Info(const std::string& format, Args ...args)
{
	spdlog::info(format, args...);
}

template<typename ...Args>
inline void Logger::Debug(const std::string& format, Args ...args)
{
	spdlog::debug(format, args...);
}

template<typename ...Args>
inline void Logger::Critical(const std::string& format, Args ...args)
{
	spdlog::critical(format, args...);
}

template<typename ...Args>
inline void Logger::Warning(const std::string& format, Args ...args)
{
	spdlog::warn(format, args...);
}
