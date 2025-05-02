#pragma once
#include "FileLogger.h"

/** Defines macros for log levels to simplify the usage of ELogLevel enum */
#define LOGLEVEL_ERROR ELogLevel::Error
#define LOGLEVEL_WARNING ELogLevel::Warning
#define LOGLEVEL_LOG ELogLevel::Log

/** Macro to log a message with the specified log level. */
#define LOG_BODY(LogLevel, Message) FileLogger::GetInstance()->Log(LogLevel, Message);

/** Logging system configuration based on LOG_LEVEL*/
/** ---------------------------------------------- */
/** Error-only logging mode */
#if LOG_LEVEL == 1 

	/** Disable standard logs */
	#undef S_LOG(Message) LOG_BODY(LOGLEVEL_LOG, Message)

	/** Disable warning logs */
	#undef S_LOG_WARNING(Message) LOG_BODY(LOGLEVEL_WARNING, Message)

	/** Enable error logs */
	#define S_LOG_ERROR(Message) LOG_BODY(LOGLEVEL_ERROR, Message)	

/** Standard logging mode (no errors) */
#elif LOG_LEVEL == 2

	/** Disable standard logs */
	#undef S_LOG(Message) LOG_BODY(LOGLEVEL_LOG, Message)

	/** Enable warning logs */
	#define S_LOG_WARNING(Message) LOG_BODY(LOGLEVEL_WARNING, Message)

	/** Enable error logs */
	#define S_LOG_ERROR(Message) LOG_BODY(LOGLEVEL_ERROR, Message)	

/** Verbose debug logging mode */
#elif LOG_LEVEL == 3 

	/** Enable standard logs */
	#define S_LOG(Message) LOG_BODY(LOGLEVEL_LOG, Message)

	/** Enable warning logs */
	#define S_LOG_WARNING(Message) LOG_BODY(LOGLEVEL_WARNING, Message)

	/** Enable error logs */
	#define S_LOG_ERROR(Message) LOG_BODY(LOGLEVEL_ERROR, Message)	

#endif