#include "SolitaireEnginePCH.h"
#include "Logger/FileLogger.h"
#include <iostream>
#include <conio.h>

FileLogger::FileLogger()
{
	//Open (create) and generate LogFile Name
	LogFileWriter = SFileWriter(GenerateLogFileName(), std::ios::out | std::ios::ate);
	
	// Check if the LogFileWriter is open
	if (!LogFileWriter.is_open())
	{
		// Display an error message to the user
		std::cout << "Failed to initialize File Logger. The log file could not be opened. Press any key to exit.\n";

		// Wait for user input before exiting (you can replace this with a more meaningful condition if needed)
		_getch();

		// Exit the program with an error code (1 indicates failure)
		exit(1);
	}

	//Create thread
	WorkerThread = SThread(&FileLogger::ProcessQueue, this);
}

FileLogger* FileLogger::GetInstance()
{
	// Return the raw pointer to the logger instance
	static SUniquePtr<FileLogger> Logger = std::make_unique<FileLogger>();
	return Logger.get();
}

void FileLogger::LogLevelToString(ELogLevel LogLevel, SWString& OutString)
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

void FileLogger::Log(ELogLevel LogLevel, const SWString& Message)
{
	SWString StringLogLevel;
	SWString StringCurrentTime;

	// Convert the log level enum to a string representation (e.g., "INFO", "ERROR")
	LogLevelToString(LogLevel, StringLogLevel);

	// Get the current time as a string using the format "HH-MM-SS"
	TimeLibrary::GetCurrentTimeAsString(TEXT("%H_%M_%S"), StringCurrentTime);

	// Combine the time, log level, and message into one formatted log entry
	SWString FullMessage = TEXT("[") + StringCurrentTime + TEXT("]") + TEXT("[") + StringLogLevel + TEXT("]") + TEXT(": ") + Message;

	{
		// Lock the mutex to safely access the log queue
		std::lock_guard<SMutex> Lock(QueueMutex);
		
		// Convert the wide string to a narrow string and enqueue it for logging
		LogQueue.push(std::move(StringLibrary::WideStringToString(FullMessage)));
	}

	// Notify the background logging thread that a new message is available
	Queue_CV.notify_one();
}


void FileLogger::ProcessQueue()
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

SString FileLogger::GenerateLogFileName()
{
	// Get the current time as a string.
	SString Time;
	TimeLibrary::GetCurrentTimeAsString(SString("%H_%M_%S"), Time);

	// Create a path object for the "Saved" directory in the project root path.
	fs::path LogPath = Core::Paths::GProjectRootPath + TEXT("Saved");

	// Check if the "Saved" directory exists, if not, create it.
	if (!fs::exists(LogPath))
	{
		fs::create_directory(LogPath); // Creates the directory if it doesn't exist.
	}
	
	// Prefix the formatted time with "Log_" to create the log file name.
	// Return the full path for the log file (including the file name).
	return LogPath.string() + "\\Log_" + Time + ".txt";;// Use LogPath here instead of GProjectRootPath directly.
}

void FileLogger::WaitForLoggingToFinish()
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