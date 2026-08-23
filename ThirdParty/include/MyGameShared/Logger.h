#pragma once
#include <spdlog/spdlog.h>

namespace MGSL::Shared
{
	class Logger
	{
		MGSL_DECLARE_SINGLE(Logger);
		using SPDLoggerPtr = SharedPtr<spdlog::logger>;

	private:
		Logger();
		~Logger();

#ifdef _DEBUG
		static SPDLoggerPtr m_logger;
#endif

#ifdef _DEBUG
	public:
		template<typename... Args>
		static void _Internal_LogTrace(fmt::format_string<Args...> fmt, Args&&... args);

		template<typename... Args>
		static void _Internal_LogInfo(fmt::format_string<Args...> fmt, Args&&... args);

		template<typename... Args>
		static void _Internal_LogWarn(fmt::format_string<Args...> fmt, Args&&... args);

		template<typename... Args>
		static void _Internal_LogError(fmt::format_string<Args...> fmt, Args&&... args);

		template<typename... Args>
		static void _Internal_LogFatal(fmt::format_string<Args...> fmt, Args&&... args);

		static void _Internal_Flush();
#endif
	};
}

/*=======================//
//     Debug Logging     //
//=======================*/
#ifdef _DEBUG
#define MGSL_LOG_FLUSH()         MGSL::Shared::Logger::_Internal_Flush()
#define MGSL_LOG_TRACE(...)      MGSL::Shared::Logger::_Internal_LogTrace(__VA_ARGS__)
#define MGSL_LOG_INFO(...)       MGSL::Shared::Logger::_Internal_LogInfo(__VA_ARGS__)
#define MGSL_LOG_WARN(...)       MGSL::Shared::Logger::_Internal_LogWarn(__VA_ARGS__)
#define MGSL_LOG_ERROR(...)      MGSL::Shared::Logger::_Internal_LogError(__VA_ARGS__)
#define MGSL_LOG_FATAL(...)      MGSL::Shared::Logger::_Internal_LogFatal(__VA_ARGS__)
#else
#define MGSL_LOG_FLUSH()         ((void)0)
#define MGSL_LOG_TRACE(...)      ((void)0)
#define MGSL_LOG_INFO(...)       ((void)0)
#define MGSL_LOG_WARN(...)       ((void)0)
#define MGSL_LOG_ERROR(...)      ((void)0)
#define MGSL_LOG_FATAL(...)      ((void)0)
#endif

#include "Logger.inl"

