#pragma once

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

namespace gp1::format
{
	template <typename T>
	struct Formatter
	{
		std::string format(const T& value, const std::string& options)
		{
			return (std::ostringstream() << value).str();
		}
	};

	namespace detail
	{
		struct DefaultFormatData
		{
		public:
			struct FillAndAlign
			{
			public:
				enum class Align
				{
					Left,
					Center,
					Right
				};

				char  m_Fill    = ' ';
				Align m_Align   = Align::Left;
				bool  m_Present = false;
			};

			enum class Sign
			{
				Pluss,
				Minus,
				Space
			};

			FillAndAlign m_FillAndAlign;
			Sign         m_Sign      = Sign::Minus;
			bool         m_Alternate = false;
			bool         m_Pad       = false;

			uint32_t m_Width     = 0;
			uint32_t m_Precision = 0;

			char m_Type = '\0';

			std::string toString();
			void        parse(const std::string& options);
		};

		struct FormattedArg
		{
		public:
			std::string m_Str;
			bool        m_Formattable = true;
		};

		template <typename T>
		struct RemoveArray
		{
			using Type = T;
		};

		template <typename T, size_t N>
		struct RemoveArray<T[N]>
		{
			using Type = T*;
		};

		template <typename T, size_t N>
		struct RemoveArray<const T[N]>
		{
			using Type = const T*;
		};

		template <typename Param>
		void formatArg(size_t index, std::vector<FormattedArg>& formattedArgs, const Param& arg)
		{
			for (FormattedArg& formattedArg : formattedArgs)
			{
				if (!formattedArg.m_Formattable)
					continue;

				std::istringstream currentFormatStream { formattedArg.m_Str };
				size_t             argIndex;
				if (currentFormatStream >> argIndex && index == argIndex)
				{
					size_t      start = formattedArg.m_Str.find_first_of(':');
					std::string option;
					if (start < formattedArg.m_Str.size())
						option = formattedArg.m_Str.substr(start + 1);

					formattedArg.m_Str         = Formatter<typename RemoveArray<Param>::Type>().format(arg, option);
					formattedArg.m_Formattable = false;
				}
			}
		}
	} // namespace detail

	template <typename... Params>
	std::string format(const std::string& fmt, const Params&... args)
	{
		std::string formatStr    = fmt;
		size_t      offset       = 0;
		size_t      currentIndex = 0;
		while ((offset = formatStr.find_first_of('{', offset)) < formatStr.size())
		{
			if (offset < formatStr.size() - 1 && formatStr[offset + 1] == '{')
			{
				offset += 2;
				continue;
			}

			size_t end = offset + 1;
			for (size_t depth = 1; end < formatStr.size(); end++)
			{
				switch (formatStr[end])
				{
				case '{':
					depth++;
					break;
				case '}':
					depth--;
					break;
				}
				if (depth == 0)
					break;
			}

			offset++;
			size_t             argIndex = formatStr.find_first_of(':', offset);
			std::istringstream currentFormatStream { formatStr.substr(offset, (argIndex < end ? argIndex : end) - offset) };
			size_t             argVal;
			if (!(currentFormatStream >> argVal))
			{
				std::string currentIndexStr = std::to_string(currentIndex++);
				formatStr                   = formatStr.substr(0, offset) + currentIndexStr + formatStr.substr(offset);
				offset += currentIndexStr.size();
			}
		}

		std::vector<detail::FormattedArg> formattedArgs;
		offset = 0;
		while ((offset = formatStr.find_first_of('{', offset)) < formatStr.size())
		{
			if (offset < formatStr.size() - 1 && formatStr[offset + 1] == '{')
			{
				offset += 2;
				continue;
			}

			size_t end = offset + 1;
			for (size_t depth = 1; end < formatStr.size(); end++)
			{
				switch (formatStr[end])
				{
				case '{':
					depth++;
					break;
				case '}':
					depth--;
					break;
				}
				if (depth == 0)
					break;
			}

			if (end < formatStr.size())
				formattedArgs.push_back({ formatStr.substr(offset + 1, end - 1 - offset) });
			offset = end + 1;
		}

		for (detail::FormattedArg& formattedArg : formattedArgs)
		{
			size_t begin = formattedArg.m_Str.find_first_of('{');
			if (begin >= formattedArg.m_Str.size() && !std::isdigit(formattedArg.m_Str[begin + 1]))
				continue;
			size_t end = formattedArg.m_Str.find_first_of('}');
			if (end >= formattedArg.m_Str.size())
				continue;

			formattedArg.m_Str = format<Params...>(formattedArg.m_Str, args...);
		}

		size_t index = 0;
		(detail::formatArg<Params>(index++, formattedArgs, args), ...);

		currentIndex = 0;
		for (size_t i = 0; i < formatStr.size();)
		{
			switch (formatStr[i])
			{
			case '{':
				if (i + 1 < formatStr.size() && formatStr[i + 1] == '{')
				{
					formatStr.replace(i, 2, 1, '{');
					i++;
				}
				else
				{
					size_t end = i + 1;
					for (size_t depth = 1; end < formatStr.size(); end++)
					{
						switch (formatStr[end])
						{
						case '{':
							depth++;
							break;
						case '}':
							depth--;
							break;
						}
						if (depth == 0)
							break;
					}

					i++;
					size_t             argIndex = formatStr.find_first_of(':', i);
					std::istringstream currentFormatStream { formatStr.substr(i, (argIndex < end ? argIndex : end) - i) };
					size_t             argVal;
					if (currentFormatStream >> argVal)
					{
						detail::FormattedArg& arg = formattedArgs[currentIndex++];
						formatStr.replace(i - 1, end - (i - 2), arg.m_Str);
						i = i - 1 + arg.m_Str.size();
					}
					else
					{
						i = end + 1;
					}
				}
				break;
			case '}':
				if (i + 1 < formatStr.size() && formatStr[i + 1] == '}')
					formatStr.replace(i, 2, 1, '}');
				i++;
				break;
			default:
				i++;
				break;
			}
		}
		return formatStr;
	}

	template <typename... Params>
	size_t formattedLength(const std::string& fmt, const Params&... args)
	{
		return format<Params...>(fmt, args...).size();
	}
} // namespace gp1::format

#include "Engine/Utility/Format/ChronoFormatters.h"
#include "Engine/Utility/Format/Formatters.h"