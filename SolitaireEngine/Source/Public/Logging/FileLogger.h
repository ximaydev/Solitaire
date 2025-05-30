#pragma once
#include "Globals.h"

/** Defines the log level enum to specify the severity of log messages. */
enum class SOLITAIRE_ENGINE_API ELogLevel : SUInt8
{
	Error = 1,   // Critical error message
	Warning,     // Warning message
	Log          // General log message
};

/** class for asynchronous logging */
class SOLITAIRE_ENGINE_API SFileLogger
{
public:
	/** Default constructor */
	SFileLogger();

	/** Returns a pointer to the global logger instance (singleton-like access) */
	static SFileLogger* GetInstance();

	/** Converts a log level enum to a corresponding string */
	void LogLevelToString(ELogLevel LogLevel, SWString& OutString) const;

	/** Logs a message */
	void Log(const SWStringView& Category, ELogLevel LogLevel, const SWideChar* const Format, ...);

	/** Waits for the logging thread to finish by waiting for the log queue to be empty */
	void WaitForLoggingToFinish();

protected:
	/** Starts processing the log message queue (runs in background thread) */
	virtual void ProcessQueue();

	/** Indicates whether the logging thread should continue running */
	SAtomic<bool> IsRunning = true;

	/** File writer used for writing to the log file */
	SWFileWriter LogFileWriter = {};

	/** Thread-safe queue of log messages */
	SQueue<SWString> LogQueue = {};

	/** Condition variable for signaling new messages */
	SConditionVariable Queue_CV = {};

	/** Mutex for synchronizing access to the queue */
	SMutex QueueMutex = {};

	/** Background thread responsible for processing log messages */
	SThread WorkerThread = {};

private:
	/** Generates a unique log file name based on the current date and time. */
	SWString GenerateLogFileName() const;
};