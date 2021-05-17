//
//	Created by FancyKillerPanda on 8. Oct. 2020
//	Edited by MarcasRealAccount on 17. Oct. 2020
//

#include <filesystem>
#include <fstream>
#include <iostream>

#include "Engine/Utility/Logger.h"

namespace gp1
{
	static LogSeverity s_ErrorSeverity = { "Error", logColors::Error, 0 };
	static LogSeverity s_WarnSeverity  = { "Warn", logColors::Warn, 10 };
	static LogSeverity s_DebugSeverity = { "Debug", logColors::Debug, 50 };
	static LogSeverity s_InfoSeverity  = { "Info", logColors::Info, 100 };

	const LogSeverity& GetLogSeverity(ELogSeverity severity)
	{
		switch (severity)
		{
		case ELogSeverity::Error: return s_ErrorSeverity;
		case ELogSeverity::Warn: return s_WarnSeverity;
		case ELogSeverity::Debug: return s_DebugSeverity;
		case ELogSeverity::Info: return s_InfoSeverity;
		default: return s_InfoSeverity;
		}
	}

	LogMessage::LogMessage(const std::string& category, const LogSeverity& severity, const std::string& message, bool newLine)
	    : m_Time(std::chrono::system_clock::now()), m_Category(category), m_Severity(severity), m_Message(message), m_NewLine(newLine) {}

	std::unordered_set<std::string> Logger::s_DisabledSeverities;
	std::vector<LogMessage>         Logger::s_Buffer;
#ifdef false // TODO(MarcasRealAccount): Either Enable of remove if we want to keep the logs in release mode with a custom log view.
	std::unordered_map<std::string, std::vector<LogMessage>> Logger::s_Logs;
#endif

	bool Logger::s_LogToFile = true;
#ifdef _DEBUG
	bool Logger::s_LogToConsole = true;
#else
	bool Logger::s_LogToConsole = false;
#endif
	const char* Logger::s_PreviousFile = "logs/log-previous.txt";
	const char* Logger::s_CurrentFile  = "logs/log-current.txt";

	Logger::Logger(const std::string& name)
	    : m_Name(name) {}

	void Logger::Init()
	{
		if (std::filesystem::exists(Logger::s_CurrentFile))
		{
			if (std::filesystem::exists(Logger::s_PreviousFile)) std::filesystem::remove(Logger::s_PreviousFile);

			if (rename(Logger::s_CurrentFile, Logger::s_PreviousFile))
				Logger("Logger").LogDebug("Failed to rename log file {} to {}", Logger::s_CurrentFile, Logger::s_PreviousFile);
		}
	}

	void Logger::DeInit()
	{
		Flush();
#ifdef false // TODO(MarcasRealAccount): Either Enable of remove if we want to keep the logs in release mode with a custom log view.
		ClearLogs();
#endif
	}

	void Logger::EnableSeverity(const LogSeverity& severity)
	{
		Logger::s_DisabledSeverities.erase(severity.m_Name);
	}

	void Logger::DisableSeverity(const LogSeverity& severity)
	{
		Logger::s_DisabledSeverities.insert(severity.m_Name);
	}

	bool Logger::IsSeverityDisabled(const LogSeverity& severity)
	{
		return Logger::s_DisabledSeverities.find(severity.m_Name) != Logger::s_DisabledSeverities.end();
	}

	void Logger::Flush()
	{
		if (!Logger::s_LogToFile) return;

		std::filesystem::path filepath { Logger::s_CurrentFile };
		std::filesystem::create_directories(filepath.parent_path());

		std::ofstream file(Logger::s_CurrentFile, std::ios::app);
		if (file.is_open())
		{
			for (auto& message : Logger::s_Buffer)
				file << format::format("{:f}\n", message);
			file.close();
			Logger::s_Buffer.clear();
		}
		else
		{
			Logger::s_LogToFile = false;
		}
	}

#ifdef false // TODO(MarcasRealAccount): Either Enable of remove if we want to keep the logs in release mode with a custom log view.
	std::vector<LogMessage> Logger::GetLog(const std::string& category)
	{
		auto itr = s_Logs.find(category);
		if (itr != s_Logs.end())
			return itr->second;
		return {};
	}

	void Logger::ClearLogs()
	{
		s_Logs.clear();
	}

	void Logger::ClearLog(const std::string& category)
	{
		auto itr = s_Logs.find(category);
		if (itr != s_Logs.end())
			itr->second.clear();
	}
#endif

	void Logger::Log(const LogMessage& message)
	{
		if (IsSeverityDisabled(message.m_Severity))
			return;

#ifdef false // TODO(MarcasRealAccount): Either Enable of remove if we want to keep the logs in release mode with a custom log view.
		auto itr = s_Logs.find(category);
		if (itr == s_Logs.end())
			itr = s_Logs.insert({ category, {} }).first;
		itr->second.push_back(message);
#endif
		std::cout << format::format("{}\n", message);
		if (s_LogToFile)
		{
			s_Buffer.push_back(message);
			if (s_Buffer.size() > message.m_Severity.m_MaxBufferSize)
				Flush();
		}
	}

	std::string format::Formatter<LogColor>::format(const LogColor& color, const std::string& options)
	{
		if (options.find_first_of('b') < options.size())
		{
			if (options.find_first_of('r') < options.size())
				return format::format("\033[49m");
			else
				return format::format("\033[48;2;{};{};{}m", color.m_R, color.m_G, color.m_B);
		}
		else
		{
			if (options.find_first_of('r') < options.size())
				return format::format("\033[39m");
			else
				return format::format("\033[38;2;{};{};{}m", color.m_R, color.m_G, color.m_B);
		}
	}

	std::string format::Formatter<LogMessage>::format(const LogMessage& message, const std::string& options)
	{
		if (options.find_first_of('f') < options.size())
		{
			if (message.m_NewLine)
				return format::format("{}{}", std::string(format::formattedLength("[{:%H:%M:%S}] {} ({}): ", message.m_Time, message.m_Category, message.m_Severity.m_Name), ' '), message.m_Message);
			else
				return format::format("[{:%H:%M:%S}] {} ({}): {}", message.m_Time, message.m_Category, message.m_Severity.m_Name, message.m_Message);
		}
		else
		{
			if (message.m_NewLine)
				return format::format("{}{}{}{0:r}", message.m_Severity.m_Color, std::string(format::formattedLength("[{:%H:%M:%S}] {} ({}): ", message.m_Time, message.m_Category, message.m_Severity.m_Name), ' '), message.m_Message);
			else
				return format::format("{}[{:%H:%M:%S}] {} ({}): {}{0:r}", message.m_Severity.m_Color, message.m_Time, message.m_Category, message.m_Severity.m_Name, message.m_Message);
		}
	}
} // namespace gp1
