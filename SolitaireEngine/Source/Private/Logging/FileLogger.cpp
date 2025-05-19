#include "SolitaireEnginePCH.h"

SFileLogger::SFileLogger()
{
	//Open (create) and generate LogFile Name
	LogFileWriter = SWFileWriter(GenerateLogFileName(), std::ios::out | std::ios::ate);
	
	// Check if the LogFileWriter is open
	if (!LogFileWriter.is_open())
	{
		// Exit the program with an error code
		assert(false && "Failed to initialize File Logger");
	}

	//Create thread
	WorkerThread = SThread(&SFileLogger::ProcessQueue, this);
}

SFileLogger* SFileLogger::GetInstance()
{
	// Return the raw pointer to the logger instance
	static SUniquePtr<SFileLogger> Logger = std::make_unique<SFileLogger>();
	return Logger.get();
}

void SFileLogger::LogLevelToString(ELogLevel LogLevel, SWString& OutString) const
{
	switch (LogLevel)
	{
		case ELogLevel::Error:
		{
			OutString = TEXT("Error"); // Set string to "Error"
			break;
		}
		case ELogLevel::Warning:
		{
			OutString = TEXT("Warning"); // Set string to "Warning"
			break;
		}
		case ELogLevel::Log:
		{
			OutString = TEXT("Log"); // Set string to "Log"
			break;
		}
		default:
		{
			OutString = TEXT("Unkown"); // Set string to "Unknown" for any unsupported log level
			break;
		}
	}
}

void SFileLogger::Log(const SWStringView& Category, ELogLevel LogLevel, const SWideChar* const Format, ...)
{
	SWString LogCategory = Category.data();
	SWString StringLogLevel;
	SWString StringCurrentTime;
	SWString Message;

	// Convert the log level enum to a string representation (e.g., "INFO", "ERROR")
	LogLevelToString(LogLevel, StringLogLevel);

	// Get the current time as a string using the format "HH-MM-SS"
	SStringLibrary::GetCurrentTimeAsString(TEXT("%H_%M_%S"), StringCurrentTime);

	va_list args;
	va_start(args, Format);

	// Calculate the required buffer size for the formatted string
	const SInt32 Length = _vscwprintf(Format, args) + 1; // +1 terminating '\0'

	// Allocate buffer for the wide string
	SWideChar* Buffer = (SWideChar*)malloc(Length * sizeof(SWideChar));

	if (Buffer != nullptr)
	{
		// Format the string safely into the buffer
		vswprintf_s(Buffer, Length, Format, args);

		// Clean up original va_list
		va_end(args);

		// Assign to SWString
		Message = Buffer;

		// Free the temporary buffer
		free(Buffer);
	}
	else
	{
		va_end(args);
	}

	// Combine the log category, time, log level, and message into one formatted log entry
	SWString FullMessage = TEXT("[") + LogCategory + TEXT("]") +
						   TEXT("[") + StringLogLevel + TEXT("]") +
						   TEXT("[") + StringCurrentTime + TEXT("]") +
						   TEXT(": ") + Message;

	{
		// Lock the mutex to safely access the log queue
		std::lock_guard<SMutex> Lock(QueueMutex);
		
		// Convert the wide string to a narrow string and enqueue it for logging
		LogQueue.push(std::move(FullMessage));
	}

	// Notify the background logging thread that a new message is available
	Queue_CV.notify_one();
}

void SFileLogger::WaitForLoggingToFinish()
{
	{
		// Acquire lock to access the queue and state safely
		std::unique_lock<std::mutex> Lock(QueueMutex);

		// Signal the logger thread to finish
		IsRunning = false;

		// Wake up the logger thread if it's waiting
		Queue_CV.notify_all();
	}

	// Wait for the logger thread to finish
	if (WorkerThread.joinable())
	{
		WorkerThread.join();
	}

	// Close the log file
	LogFileWriter.close();
}

void SFileLogger::ProcessQueue()
{
	while (IsRunning || !LogQueue.empty())
	{
		// Acquire the lock to access the queue safely
		std::unique_lock<SMutex> Lock(QueueMutex);
			
		// Wait until there is a message in the queue or logging is stopping
		Queue_CV.wait(Lock, [this]() { return !LogQueue.empty() || !IsRunning; });

		// Process all messages currently in the queue
		while (!LogQueue.empty())
		{
			// Write the message to the log file
			LogFileWriter << LogQueue.front() << '\n';

			// Force the log data to be written to the file immediately
			LogFileWriter.flush();

			// Remove the processed message from the queue
			LogQueue.pop();	
		}
	}
}

SWString SFileLogger::GenerateLogFileName() const
{
	// Get the current time as a string.
	SWString Time;
	SStringLibrary::GetCurrentTimeAsString(SWString(TEXT("%H_%M_%S")), Time);
	
	// Prefix the formatted time with "Log_" to create the log file name.
	// Return the full path for the log file (including the file name).
	return Core::Paths::GetProjectSavedPath() + TEXT("Log_") + Time + TEXT(".txt");
}