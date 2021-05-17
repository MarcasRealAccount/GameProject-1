#pragma once

#include "Engine/Utility/Format/Format.h"

template <>
struct gp1::format::Formatter<std::string>
{
	std::string format(const std::string& value, const std::string& option);
};

template <>
struct gp1::format::Formatter<uint64_t>
{
	std::string format(uint64_t value, const std::string& option);
};

template <>
struct gp1::format::Formatter<int64_t>
{
	std::string format(int64_t value, const std::string& option);
};

template <>
struct gp1::format::Formatter<char>
{
	std::string format(char value, const std::string& option);
};

template <>
struct gp1::format::Formatter<bool>
{
	std::string format(bool value, const std::string& option);
};

template <>
struct gp1::format::Formatter<long double>
{
	std::string format(long double value, const std::string& option);
};

template <typename T>
struct gp1::format::Formatter<T*>
{
	std::string format(T* value, const std::string& option)
	{
		detail::DefaultFormatData data;
		data.m_FillAndAlign.m_Align = detail::DefaultFormatData::FillAndAlign::Align::Right;
		data.parse(option);
		switch (data.m_Type)
		{
		case '\0':
			[[fallthrough]];
		case 'p':
			data.m_Type = 'x';
			break;
		case 'p':
			data.m_Type = 'X';
			break;
		}
		std::string uoptions = data.toString();
		return gp1::format::Formatter<uintptr_t>().format(static_cast<uintptr_t>(value), uoptions);
	}
};

template <>
struct gp1::format::Formatter<const char*>
{
	inline std::string format(const char* value, const std::string& option)
	{
		return gp1::format::Formatter<std::string>().format(value, option);
	}
};

template <>
struct gp1::format::Formatter<char*>
{
	inline std::string format(const char* value, const std::string& option)
	{
		return gp1::format::Formatter<std::string>().format(value, option);
	}
};

template <>
struct gp1::format::Formatter<uint32_t>
{
	inline std::string format(uint32_t value, const std::string& option)
	{
		return gp1::format::Formatter<uint64_t>().format(value, option);
	}
};

template <>
struct gp1::format::Formatter<uint16_t>
{
	inline std::string format(uint16_t value, const std::string& option)
	{
		return gp1::format::Formatter<uint64_t>().format(value, option);
	}
};

template <>
struct gp1::format::Formatter<uint8_t>
{
	inline std::string format(uint8_t value, const std::string& option)
	{
		return gp1::format::Formatter<uint64_t>().format(value, option);
	}
};

template <>
struct gp1::format::Formatter<int32_t>
{
	inline std::string format(int32_t value, const std::string& option)
	{
		return gp1::format::Formatter<int64_t>().format(value, option);
	}
};

template <>
struct gp1::format::Formatter<int16_t>
{
	inline std::string format(int16_t value, const std::string& option)
	{
		return gp1::format::Formatter<int64_t>().format(value, option);
	}
};

template <>
struct gp1::format::Formatter<int8_t>
{
	inline std::string format(int8_t value, const std::string& option)
	{
		return gp1::format::Formatter<int64_t>().format(value, option);
	}
};

template <>
struct gp1::format::Formatter<double>
{
	inline std::string format(double value, const std::string& option)
	{
		return gp1::format::Formatter<long double>().format(value, option);
	}
};

template <>
struct gp1::format::Formatter<float>
{
	inline std::string format(float value, const std::string& option)
	{
		return gp1::format::Formatter<long double>().format(value, option);
	}
};