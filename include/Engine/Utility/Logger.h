//
//	Created by FancyKillerPanda on 8. Oct. 2020
//	Edited by MarcasRealAccount on 17. Oct. 2020
//

#pragma once

#include "Engine/Utility/Format/Format.h"

#include <chrono>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace gp1
{
	struct LogColor
	{
	public:
		constexpr LogColor(uint8_t r, uint8_t g, uint8_t b)
		    : m_R(r), m_G(g), m_B(b) {}

	public:
		uint8_t m_R;
		uint8_t m_G;
		uint8_t m_B;
	};

	namespace logColors
	{
		static constexpr const LogColor White  = { 255, 255, 255 };
		static constexpr const LogColor Red    = { 255, 0, 0 };
		static constexpr const LogColor Yellow = { 255, 255, 0 };
		static constexpr const LogColor Green  = { 0, 255, 0 };
		static constexpr const LogColor Cyan   = { 0, 255, 255 };
		static constexpr const LogColor Blue   = { 0, 0, 255 };
		static constexpr const LogColor Purple = { 255, 0, 255 };
		static constexpr const LogColor Black  = { 0, 0, 0 };

		static constexpr const LogColor Error = { 255, 30, 30 };
		static constexpr const LogColor Warn  = { 255, 200, 50 };
		static constexpr const LogColor Debug = { 80, 165, 255 };
		static constexpr const LogColor Info  = White;
	} // namespace logColors

	struct LogSeverity
	{
	public:
		std::string m_Name;
		LogColor    m_Color;
		size_t      m_MaxBufferSize;
	};

	enum class ELogSeverity
	{
		Error,
		Warn,
		Debug,
		Info
	};

	const LogSeverity& GetLogSeverity(ELogSeverity severity);

	struct LogMessage
	{
	public:
		LogMessage(const std::string& category, const LogSeverity& severity, const std::string& message, bool newLine = false);

	public:
		std::chrono::system_clock::time_point m_Time;

		std::string m_Category;
		LogSeverity m_Severity;
		std::string m_Message;
		bool        m_NewLine;
	};

	class Logger
	{
	public:
		Logger(const std::string& name);

		// Log a message with the given severity and message.
		template <typename... Params>
		inline void Log(const LogSeverity& severity, const std::string& format, const Params&... args)
		{
			Log<Params...>(m_Name, severity, format, args...);
		}

		// Log a message with the given severity and message.
		template <typename... Params>
		inline void Log(ELogSeverity severity, const std::string& format, const Params&... args)
		{
			return Log<Params...>(GetLogSeverity(severity), format, args...);
		}

		// Log a trace severity message.
		template <typename... Params>
		inline void LogInfo(const std::string& format, const Params&... args)
		{
			return Log<Params...>(ELogSeverity::Info, format, args...);
		}

		// Log a debug severity message.
		template <typename... Params>
		inline void LogDebug(const std::string& format, const Params&... args)
		{
			return Log<Params...>(ELogSeverity::Debug, format, args...);
		}

		// Log a warning severity message.
		template <typename... Params>
		inline void LogWarn(const std::string& format, const Params&... args)
		{
			return Log<Params...>(ELogSeverity::Warn, format, args...);
		}

		// Log an error severity message.
		template <typename... Params>
		inline void LogError(const std::string& format, const Params&... args)
		{
			return Log<Params...>(ELogSeverity::Error, format, args...);
		}

#ifdef false // TODO(MarcasRealAccount): Either Enable of remove if we want to keep the logs in release mode with a custom log view.
		inline const std::vector<LogMessage>& GetLog() const
		{
			return GetLog(m_Name);
		}

		inline void ClearLog()
		{
			ClearLog(m_Name);
		}
#endif

	public:
		// Initialize the logger system.
		static void Init();
		// Deinitialize the logger system.
		static void DeInit();

		// Enable the given severity to be logged.
		static void        EnableSeverity(const LogSeverity& severity);
		inline static void EnableSeverity(ELogSeverity severity)
		{
			EnableSeverity(GetLogSeverity(severity));
		}

		// Disable the given severity from being logged.
		static void        DisableSeverity(const LogSeverity& severity);
		inline static void DisableSeverity(ELogSeverity severity)
		{
			DisableSeverity(GetLogSeverity(severity));
		}

		static bool        IsSeverityDisabled(const LogSeverity& severity);
		inline static bool IsSeverityDisabled(ELogSeverity severity)
		{
			return IsSeverityDisabled(GetLogSeverity(severity));
		}

		// Flush the logger system's buffer.
		static void Flush();

#ifdef false // TODO(MarcasRealAccount): Either Enable of remove if we want to keep the logs in release mode with a custom log view.
		inline static const std::unordered_map<std::string, std::vector<LogMessage>>& GetLogs()
		{
			return s_Logs;
		}

		static std::vector<LogMessage> GetLog(const std::string& category);

		static void ClearLogs();
		static void ClearLog(const std::string& category);
#endif

		// Log to the console and to a file.
		template <typename... Params>
		static void Log(const std::string& category, const LogSeverity& severity, const std::string& format, const Params&... args)
		{
			std::string      message = format::format<Params...>(format, args...);
			std::string_view mainMessage(message);

			std::vector<std::string_view> messages;

			size_t ci = 0;
			for (size_t i = 0; i < mainMessage.size(); i++)
			{
				char c = mainMessage[i];
				switch (c)
				{
				case '\r':
					[[fallthrough]];
				case '\n':
					if ((i - ci) > 0)
						messages.push_back(mainMessage.substr(ci, i - ci));
					ci = i + 1;
					break;
				}
				if (i == mainMessage.size() - 1)
					messages.push_back(mainMessage.substr(ci));
			}

			ci = 0;
			for (auto& msg : messages)
				Log(LogMessage(category, severity, static_cast<std::string>(msg), static_cast<bool>(ci++)));
		}

	private:
		static void Log(const LogMessage& message);

	private:
		std::string m_Name; // The name of this logger.

	private:
		static std::unordered_set<std::string> s_DisabledSeverities; // The disabled severities.

		static std::vector<LogMessage> s_Buffer; // The buffer to hold messages for writing to a file.

#ifdef false // TODO(MarcasRealAccount): Either Enable of remove if we want to keep the logs in release mode with a custom log view.
		static std::unordered_map<std::string, std::vector<LogMessage>> s_Logs;
#endif

		static bool        s_LogToFile;    // Can log to file.
		static bool        s_LogToConsole; // Can log to console.
		static const char* s_PreviousFile; // The previous log file name.
		static const char* s_CurrentFile;  // The current log file name.
	};

	template <>
	struct format::Formatter<LogColor>
	{
		std::string format(const LogColor& color, const std::string& options);
	};

	template <>
	struct format::Formatter<LogMessage>
	{
		std::string format(const LogMessage& message, const std::string& options);
	};
} // namespace gp1
