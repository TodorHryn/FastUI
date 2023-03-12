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

		virtual void load(const std::wstring &path);
		virtual void load(const std::vector<uint8_t> &data);

		virtual CharacterOpenGL& get(wchar_t ch);

		void operator=(const FontOpenGL&) = delete;

	private:
		static FT_Library *m_library;
		FT_Face m_face;
		std::vector<uint8_t> m_faceData;
		std::unordered_map<wchar_t, CharacterOpenGL> m_chars;
};