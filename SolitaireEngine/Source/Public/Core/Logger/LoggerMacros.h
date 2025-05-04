#pragma once
#include "FileLogger.h"

/** Defines macros for log levels to simplify the usage of ELogLevel enum */
#define LOGLEVEL_ERROR ELogLevel::Error
#define LOGLEVEL_WARNING ELogLevel::Warning
#define LOGLEVEL_LOG ELogLevel::Log

/** Macro to log a message with the specified log level. */
#define LOG_BODY(Category, LogLevel, Format, ...) FileLogger::GetInstance()->Log(Category, LogLevel, Format, __VA_ARGS__);

/** Logging system configuration based on LOG_LEVEL*/
/** ---------------------------------------------- */
/** Error-only logging mode */
#if LOG_LEVEL == 1 

	/** Disable standard logs */
	#undef S_LOG(Category, Format, ...) LOG_BODY(Category, LOGLEVEL_LOG, Format, __VA_ARGS__)

	/** Disable warning logs */
	#undef S_LOG_WARNING(Category, Format, ...) LOG_BODY(Category, LOGLEVEL_WARNING, Format, __VA_ARGS__)

	/** Enable error logs */
	#define S_LOG_ERROR(Category, Format, ...) LOG_BODY(Category, LOGLEVEL_ERROR, Format, __VA_ARGS__)	

/** Standard logging mode (no errors) */
#elif LOG_LEVEL == 2

	/** Disable standard logs */
	#undef#define S_LOG(Category, Format, ...) LOG_BODY(Category, LOGLEVEL_LOG, Format, __VA_ARGS__)

	/** Enable warning logs */
	#define S_LOG_WARNING(Category, Format, ...) LOG_BODY(Category, LOGLEVEL_WARNING, Format, __VA_ARGS__)

	/** Enable error logs */
	#define S_LOG_ERROR(Category, Format, ...) LOG_BODY(Category, LOGLEVEL_ERROR, Format, __VA_ARGS__)	

/** Verbose debug logging mode */
#elif LOG_LEVEL == 3 

	/** Enable standard logs */
	#define S_LOG(Category, Format, ...) LOG_BODY(Category, LOGLEVEL_LOG, Format, __VA_ARGS__)

	/** Enable warning logs */
	#define S_LOG_WARNING(Category, Format, ...) LOG_BODY(Category, LOGLEVEL_WARNING, Format, __VA_ARGS__)

	/** Enable error logs */
	#define S_LOG_ERROR(Category, Format, ...) LOG_BODY(Category, LOGLEVEL_ERROR, Format, __VA_ARGS__)	

#endif