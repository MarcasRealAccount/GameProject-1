#include "Engine/Utility/Format/Formatters.h"

#include <charconv>
#include <cmath>

namespace gp1::format
{
	std::string Formatter<std::string>::format(const std::string& value, const std::string& option)
	{
		detail::DefaultFormatData data;
		data.parse(option);
		std::string str = value;
		if (data.m_Width > str.size())
		{
			switch (data.m_FillAndAlign.m_Align)
			{
			case detail::DefaultFormatData::FillAndAlign::Align::Left:
				str += std::string(data.m_Width - str.size(), data.m_FillAndAlign.m_Fill);
				break;
			case detail::DefaultFormatData::FillAndAlign::Align::Center:
			{
				float  len = (float) (data.m_Width - str.size()) / 2;
				size_t ll  = (size_t) std::floor(len);
				size_t rl  = (size_t) std::ceil(len);
				str        = std::string(ll, data.m_FillAndAlign.m_Fill) + str + std::string(rl, data.m_FillAndAlign.m_Fill);
				break;
			}
			case detail::DefaultFormatData::FillAndAlign::Align::Right:
				str = std::string(data.m_Width - str.size(), data.m_FillAndAlign.m_Fill) + str;
				break;
			}
		}
		return str;
	}

	std::string Formatter<uint64_t>::format(uint64_t value, const std::string& option)
	{
		detail::DefaultFormatData tempData;
		detail::DefaultFormatData data;
		tempData.parse(option);
		if (tempData.m_Type == 'c')
			data.m_FillAndAlign.m_Align = detail::DefaultFormatData::FillAndAlign::Align::Left;
		else
			data.m_FillAndAlign.m_Align = detail::DefaultFormatData::FillAndAlign::Align::Right;
		data.parse(option);

		auto padWithLeadingZeros = [](std::string& str, size_t num) {
			str = std::string(num, '0') + str;
		};

		std::string str;
		char        buffer[64] { '\0' };
		char*       end       = buffer + sizeof(buffer);
		bool        upperCase = false;
		switch (data.m_Type)
		{
		case 'B':
			upperCase = true;
			[[fallthrough]];
		case 'b':
			std::to_chars(buffer, end, value, 2);
			str = buffer;
			if (!data.m_FillAndAlign.m_Present && data.m_Pad && (str.size() + (data.m_Alternate ? 2 : 0)) < data.m_Width)
				padWithLeadingZeros(str, data.m_Width - str.size() - (data.m_Alternate ? 2 : 0));
			if (data.m_Alternate)
			{
				if (upperCase)
					str = "0B" + str;
				else
					str = "0b" + str;
			}
			break;
		case 'c':
			str += static_cast<char>(value);
			break;
		case '\0':
			[[fallthrough]];
		case 'd':
			std::to_chars(buffer, end, value);
			str = buffer;
			if (!data.m_FillAndAlign.m_Present && data.m_Pad && (str.size() + (data.m_Sign == detail::DefaultFormatData::Sign::Pluss || data.m_Sign == detail::DefaultFormatData::Sign::Space ? 1 : 0)) < data.m_Width)
				padWithLeadingZeros(str, data.m_Width - str.size() - (data.m_Sign == detail::DefaultFormatData::Sign::Pluss || data.m_Sign == detail::DefaultFormatData::Sign::Space ? 1 : 0));
			switch (data.m_Sign)
			{
			case detail::DefaultFormatData::Sign::Pluss:
				str = "+" + str;
				break;
			case detail::DefaultFormatData::Sign::Space:
				str = " " + str;
				break;
			default:
				break;
			}
			break;
		case 'o':
			std::to_chars(buffer, end, value, 8);
			str = buffer;
			if (!data.m_FillAndAlign.m_Present && data.m_Pad && (str.size() + (data.m_Alternate ? 1 : 0)) < data.m_Width)
				padWithLeadingZeros(str, data.m_Width - str.size() - (data.m_Alternate ? 1 : 0));
			if (data.m_Alternate)
			{
				if (value != 0)
					str = "0" + str;
			}
			break;
		case 'X':
			upperCase = true;
			[[fallthrough]];
		case 'x':
			std::to_chars(buffer, end, value, 16);
			str = buffer;
			if (upperCase)
				for (size_t i = 0; i < str.size(); i++)
					str[i] = (char) std::toupper(str[i]);
			if (!data.m_FillAndAlign.m_Present && data.m_Pad && (str.size() + (data.m_Alternate ? 2 : 0)) < data.m_Width)
				padWithLeadingZeros(str, data.m_Width - str.size() - (data.m_Alternate ? 2 : 0));
			if (data.m_Alternate)
			{
				if (upperCase)
					str = "0X" + str;
				else
					str = "0x" + str;
			}
			break;
		}

		if (!(!data.m_FillAndAlign.m_Present && data.m_Pad) && data.m_Width > str.size())
		{
			switch (data.m_FillAndAlign.m_Align)
			{
			case detail::DefaultFormatData::FillAndAlign::Align::Left:
				str += std::string(data.m_Width - str.size(), data.m_FillAndAlign.m_Fill);
				break;
			case detail::DefaultFormatData::FillAndAlign::Align::Center:
			{
				float  len = (float) (data.m_Width - str.size()) / 2;
				size_t ll  = (size_t) std::floor(len);
				size_t rl  = (size_t) std::ceil(len);
				str        = std::string(ll, data.m_FillAndAlign.m_Fill) + str + std::string(rl, data.m_FillAndAlign.m_Fill);
				break;
			}
			case detail::DefaultFormatData::FillAndAlign::Align::Right:
				str = std::string(data.m_Width - str.size(), data.m_FillAndAlign.m_Fill) + str;
				break;
			}
		}
		return str;
	}

	std::string Formatter<int64_t>::format(int64_t value, const std::string& option)
	{
		detail::DefaultFormatData tempData;
		detail::DefaultFormatData data;
		tempData.parse(option);
		if (tempData.m_Type == 'c')
			data.m_FillAndAlign.m_Align = detail::DefaultFormatData::FillAndAlign::Align::Left;
		else
			data.m_FillAndAlign.m_Align = detail::DefaultFormatData::FillAndAlign::Align::Right;
		data.parse(option);
		bool negative = false;
		if (value < 0)
		{
			data.m_Sign = detail::DefaultFormatData::Sign::Pluss;
			value       = -value;
			negative    = true;
		}
		std::string uoptions = data.toString();

		std::string str = Formatter<uint64_t>().format(static_cast<uint64_t>(value), uoptions);
		if ((data.m_Type == '\0' || data.m_Type == 'd') && negative)
		{
			switch (data.m_FillAndAlign.m_Align)
			{
			case detail::DefaultFormatData::FillAndAlign::Align::Left:
				for (size_t i = 0; i < str.size(); i++)
				{
					if (!std::isdigit(str[i]))
					{
						str[i] = '-';
						break;
					}
				}
				break;
			case detail::DefaultFormatData::FillAndAlign::Align::Right:
				for (size_t i = str.size(); i > 0; i--)
				{
					if (!std::isdigit(str[i - 1]))
					{
						str[i - 1] = '-';
						break;
					}
				}
				break;
			case detail::DefaultFormatData::FillAndAlign::Align::Center:
				for (size_t i = str.size() / 2; i > 0; i--)
				{
					if (!std::isdigit(str[i - 1]))
					{
						str[i - 1] = '-';
						break;
					}
				}
				break;
			}
		}
		return str;
	}

	std::string Formatter<char>::format(char value, const std::string& option)
	{
		detail::DefaultFormatData tempData;
		detail::DefaultFormatData data;
		tempData.parse(option);
		if (tempData.m_Type == '\0' || tempData.m_Type == 'c')
			data.m_FillAndAlign.m_Align = detail::DefaultFormatData::FillAndAlign::Align::Left;
		else
			data.m_FillAndAlign.m_Align = detail::DefaultFormatData::FillAndAlign::Align::Right;
		data.parse(option);
		if (data.m_Type == '\0')
			data.m_Type = 'c';
		std::string uoptions = data.toString();
		return Formatter<int8_t>().format(static_cast<int8_t>(value), uoptions);
	}

	std::string Formatter<bool>::format(bool value, const std::string& option)
	{
		detail::DefaultFormatData data;
		data.parse(option);
		switch (data.m_Type)
		{
		case '\0':
			[[fallthrough]];
		case 's':
			return Formatter<const char*>().format(value ? "true" : "false", option);
		case 'b':
		case 'B':
		case 'c':
		case 'd':
		case 'o':
		case 'x':
		case 'X':
			return Formatter<uint8_t>().format(static_cast<uint8_t>(value), option);
		default:
			return "";
		}
	}

	std::string Formatter<long double>::format(long double value, const std::string& option)
	{
		detail::DefaultFormatData data;
		data.m_FillAndAlign.m_Align = detail::DefaultFormatData::FillAndAlign::Align::Right;
		data.parse(option);

		auto padWithLeadingZeros = [](std::string& str, size_t num) {
			str = std::string(num, '0') + str;
		};

		std::string str;
		char        buffer[128] { '\0' };
		char*       end       = buffer + sizeof(buffer);
		bool        upperCase = false;
		bool        negative  = value < 0;
		if (negative)
			value = -value;
		switch (data.m_Type)
		{
		case 'A':
			upperCase = true;
			[[fallthrough]];
		case 'a':
			if (data.m_Precision != 0)
				std::to_chars(buffer, end, value, std::chars_format::hex, data.m_Precision);
			else
				std::to_chars(buffer, end, value, std::chars_format::hex);
			str = buffer;
			if (data.m_Alternate && str != "inf" && str != "nan")
			{
				size_t dot = str.find_first_of('.');
				if (dot >= str.size())
				{
					size_t p = str.find_first_of('p');
					str      = str.substr(0, p) + ".0" + str.substr(p);
				}
			}
			if (upperCase)
				for (size_t i = 0; i < str.size(); i++)
					str[i] = (char) std::toupper(str[i]);
			break;
		case 'E':
			upperCase = true;
			[[fallthrough]];
		case 'e':
			std::to_chars(buffer, end, value, std::chars_format::scientific, data.m_Precision != 0 ? data.m_Precision : 6);
			str = buffer;
			if (data.m_Alternate && str != "inf" && str != "nan")
			{
				size_t dot = str.find_first_of('.');
				if (dot >= str.size())
				{
					size_t e = str.find_first_of('e');
					str      = str.substr(0, e) + ".0" + str.substr(e);
				}
			}
			if (upperCase)
				for (size_t i = 0; i < str.size(); i++)
					str[i] = (char) std::toupper(str[i]);
			break;
		case 'F':
			upperCase = true;
			[[fallthrough]];
		case 'f':
			std::to_chars(buffer, end, value, std::chars_format::fixed, data.m_Precision != 0 ? data.m_Precision : 6);
			str = buffer;
			break;
		case 'G':
			upperCase = true;
			[[fallthrough]];
		case 'g':
			std::to_chars(buffer, end, value, std::chars_format::general, data.m_Precision != 0 ? data.m_Precision : 6);
			str = buffer;
			if (data.m_Alternate && str != "inf" && str != "nan")
			{
				size_t dot           = str.find_first_of('.');
				size_t trailingZeros = 0;
				if (dot < str.size())
				{
					size_t decimals = (str.size() - dot);
					if (decimals < data.m_Precision)
						trailingZeros = data.m_Precision - decimals;
				}
				else
				{
					trailingZeros = data.m_Precision;
					str += ".";
				}
				str += std::string(trailingZeros, 0);
			}

			if (upperCase)
				for (size_t i = 0; i < str.size(); i++)
					str[i] = (char) std::toupper(str[i]);
			break;
		case '\0':
			if (data.m_Precision != 0)
				std::to_chars(buffer, end, value, std::chars_format::general, data.m_Precision);
			else
				std::to_chars(buffer, end, value);
			str = buffer;
			if (data.m_Alternate && str != "inf" && str != "nan")
			{
				size_t dot = str.find_first_of('.');
				if (dot >= str.size())
					str += ".0";
			}
			break;
		}

		if (!data.m_FillAndAlign.m_Present && data.m_Pad && (str.size() + (!negative && data.m_Sign == detail::DefaultFormatData::Sign::Minus ? 0 : 1)) < data.m_Width)
			padWithLeadingZeros(str, data.m_Width - str.size() - (!negative && data.m_Sign == detail::DefaultFormatData::Sign::Minus ? 0 : 1));

		switch (data.m_Sign)
		{
		case detail::DefaultFormatData::Sign::Pluss:
			if (negative)
				str = "-" + str;
			else
				str = "+" + str;
			break;
		case detail::DefaultFormatData::Sign::Minus:
			if (negative)
				str = "-" + str;
			break;
		case detail::DefaultFormatData::Sign::Space:
			if (negative)
				str = "-" + str;
			else
				str = " " + str;
			break;
		}

		if (!(!data.m_FillAndAlign.m_Present && data.m_Pad) && data.m_Width > str.size())
		{
			switch (data.m_FillAndAlign.m_Align)
			{
			case detail::DefaultFormatData::FillAndAlign::Align::Left:
				str += std::string(data.m_Width - str.size(), data.m_FillAndAlign.m_Fill);
				break;
			case detail::DefaultFormatData::FillAndAlign::Align::Center:
			{
				float  len = (float) (data.m_Width - str.size()) / 2;
				size_t ll  = (size_t) std::floor(len);
				size_t rl  = (size_t) std::ceil(len);
				str        = std::string(ll, data.m_FillAndAlign.m_Fill) + str + std::string(rl, data.m_FillAndAlign.m_Fill);
				break;
			}
			case detail::DefaultFormatData::FillAndAlign::Align::Right:
				str = std::string(data.m_Width - str.size(), data.m_FillAndAlign.m_Fill) + str;
				break;
			}
		}
		return str;
	}
} // namespace gp1::format