#include "Engine/Utility/Format/Format.h"

namespace gp1::format::detail
{
	std::string DefaultFormatData::toString()
	{
		std::string str;
		if (m_FillAndAlign.m_Present)
		{
			str += m_FillAndAlign.m_Fill;
			switch (m_FillAndAlign.m_Align)
			{
			case FillAndAlign::Align::Left:
				str += "<";
				break;
			case FillAndAlign::Align::Center:
				str += "^";
				break;
			case FillAndAlign::Align::Right:
				str += ">";
				break;
			}
		}

		switch (m_Sign)
		{
		case Sign::Pluss:
			str += "+";
			break;
		case Sign::Minus:
			str += "-";
			break;
		case Sign::Space:
			str += " ";
			break;
		}

		if (m_Alternate)
			str += "#";

		if (m_Pad)
			str += "0";

		str += std::to_string(m_Width) + "." + std::to_string(m_Precision) + m_Type;
		return str;
	}

	void DefaultFormatData::parse(const std::string& options)
	{
		size_t o = 0;

		size_t fillAndAlignOffset = options.find_first_of("<^>", o);
		if (fillAndAlignOffset < options.size())
		{
			o = fillAndAlignOffset + 1;
			if (fillAndAlignOffset > 0)
				m_FillAndAlign.m_Fill = options[fillAndAlignOffset - 1];
			char a = options[fillAndAlignOffset];
			switch (a)
			{
			case '<':
				m_FillAndAlign.m_Align = FillAndAlign::Align::Left;
				break;
			case '^':
				m_FillAndAlign.m_Align = FillAndAlign::Align::Center;
				break;
			case '>':
				m_FillAndAlign.m_Align = FillAndAlign::Align::Right;
				break;
			}
			m_FillAndAlign.m_Present = true;
		}

		size_t signOffset = options.find_first_of("+- ", o);
		if (signOffset < options.size())
		{
			o = signOffset + 1;
			switch (options[signOffset])
			{
			case '+':
				m_Sign = Sign::Pluss;
				break;
			case '-':
				m_Sign = Sign::Minus;
				break;
			case ' ':
				m_Sign = Sign::Space;
				break;
			}
		}

		size_t alternateOffset = options.find_first_of('#', o);
		if (alternateOffset < options.size())
		{
			o           = alternateOffset + 1;
			m_Alternate = true;
		}

		size_t end = options.size();
		size_t to  = options.find_last_of("sbBcdoxXaAeEfFgGp", end);
		if (to < options.size())
		{
			end    = to - 1;
			m_Type = options[to];
		}

		size_t precisionOffset = options.find_last_of('.', end);

		size_t padOffset = options.find_first_of('0', o);
		if (padOffset < options.size() && padOffset == o && padOffset < precisionOffset)
		{
			o     = padOffset + 1;
			m_Pad = true;
		}

		if (precisionOffset < options.size() && precisionOffset >= o)
		{
			std::istringstream iss { options.substr(precisionOffset + 1, end - precisionOffset) };
			iss >> m_Precision;
			end = precisionOffset - 1;
		}

		if (end >= o)
		{
			std::istringstream iss { options.substr(o, (end + 1) - o) };
			iss >> m_Width;
		}
	}
} // namespace gp1::format::detail