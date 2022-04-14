#pragma once
#include <ft2build.h>
#include <freetype\freetype.h>
#include <string>
#include <unordered_map>
#include "CharacterOpenGL.h"

class FontOpenGL
{
	public:
		FontOpenGL();
		FontOpenGL(const std::wstring &path);
		FontOpenGL(const FontOpenGL&) = delete;
		virtual ~FontOpenGL();

		virtual void setFontPath(const std::wstring &path);

		virtual CharacterOpenGL& get(wchar_t ch);

		void operator=(const FontOpenGL&) = delete;

	private:
		static FT_Library *m_library;
		FT_Face m_face;
		std::unordered_map<wchar_t, CharacterOpenGL> m_chars;
};