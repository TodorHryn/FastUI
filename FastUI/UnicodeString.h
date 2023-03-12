#pragma once
#include <unicode/unistr.h>

namespace fastui
{
	//Unlike in icu::UnicodeString, all indexes are always counted in codepoints
	class UnicodeString
	{
	public:
		static const size_t npos = -1;
		typedef UChar32 char_type;

		UnicodeString();
		UnicodeString(char* str);
		UnicodeString(const std::string& str);

		void erase(size_t ind);
		void insert(size_t ind, char_type ch);
		char_type pop_back();

		size_t find(char_type ch) const;
		UnicodeString& operator=(const std::string& str);
		UnicodeString& operator+=(char_type ch);
		bool operator==(const std::string& str) const;
		const char_type operator[](size_t ind) const;
		std::string toUtf8() const;
		size_t size() const;

	protected:
		int32_t cpToCu(size_t ind) const;

		icu::UnicodeString m_str;
	};
};