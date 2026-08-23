#include "SharedPch.h"
#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace MGSL::Shared
{
#ifdef _DEBUG
	Logger::SPDLoggerPtr Logger::m_logger = nullptr;

	Logger::Logger()
	{
		if (m_logger) return;

		spdlog::set_pattern("%^[%T] %n: %v%$");
		m_logger = spdlog::stdout_color_mt("MGSL");
		m_logger->set_level(spdlog::level::trace);

		_Internal_LogInfo("Logger Started. [The Beginning]");
	}

	Logger::~Logger()
	{
		if (!m_logger) return;

		_Internal_LogInfo("Logger Cleared. [The End]");
		m_logger.reset();
		spdlog::shutdown();
	}

	void Logger::_Internal_Flush()
	{
		if (m_logger) m_logger->flush();
	}
#else
	Logger::Logger() = default;
	Logger::~Logger() = default;
#endif
}