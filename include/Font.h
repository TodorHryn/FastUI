#pragma once
#include <ft2build.h>
#include <freetype/freetype.h>
#include <string>
#include <unordered_map>
#include "Character.h"

class Font
{
	public:
		Font(const std::wstring &path);
		virtual ~Font();

		Character& get(wchar_t ch);

	private:
		static FT_Library *m_library;
		FT_Face m_face;
		std::unordered_map<wchar_t, Character> m_chars;
};