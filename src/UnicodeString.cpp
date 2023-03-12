#include "UnicodeString.h"

namespace fastui
{
	UnicodeString::UnicodeString()
	{
	}

	UnicodeString::UnicodeString(char* str)
		: m_str(icu::UnicodeString::fromUTF8(str))
	{
		
	}

	UnicodeString::UnicodeString(const std::string& str)
		: m_str(icu::UnicodeString::fromUTF8(str))
	{
		
	}

	void UnicodeString::erase(size_t ind)
	{
		int32_t start = cpToCu(ind);
		int32_t end = cpToCu(ind + 1);
		m_str.remove(start, end - start);
	}

	void UnicodeString::insert(size_t ind, char_type ch)
	{
		m_str.insert(cpToCu(ind), ch);
	}

	UnicodeString::char_type UnicodeString::pop_back()
	{
		char_type ch = (*this)[size() - 1];
		erase(size() - 1);
		return ch;
	}

	size_t UnicodeString::find(char_type ch) const
	{
		return m_str.indexOf(ch);
	}

	UnicodeString& UnicodeString::operator=(const std::string& str)
	{
		m_str = icu::UnicodeString(str.c_str());
		return *this;
	}

	UnicodeString& UnicodeString::operator+=(char_type ch)
	{
		insert(size(), ch);
		return *this;
	}

	bool UnicodeString::operator==(const std::string& str) const
	{
		return m_str == icu::UnicodeString(str.c_str());
	}

	const UnicodeString::char_type UnicodeString::operator[](size_t ind) const
	{
		return m_str.char32At(cpToCu(ind));
	}

	std::string UnicodeString::toUtf8() const
	{
		std::string ret;
		m_str.toUTF8String(ret);
		return ret;
	}

	size_t UnicodeString::size() const
	{
		return m_str.countChar32();
	}

	int32_t UnicodeString::cpToCu(size_t ind) const
	{
		return m_str.moveIndex32(0, static_cast<int32_t>(ind));
	}
};