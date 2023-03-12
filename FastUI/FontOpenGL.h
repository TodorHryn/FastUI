#pragma once
#include <ft2build.h>
#include <freetype\freetype.h>
#include <string>
#include <unordered_map>
#include "CharacterOpenGL.h"
#include "UnicodeString.h"

class FontOpenGL
{
	public:
		FontOpenGL();
		FontOpenGL(const std::string &path);
		FontOpenGL(const FontOpenGL&) = delete;
		virtual ~FontOpenGL();

		virtual void load(const std::string &path);
		virtual void load(const std::vector<uint8_t> &data);

		virtual CharacterOpenGL& get(UnicodeString::char_type ch);

		void operator=(const FontOpenGL&) = delete;

	private:
		static FT_Library *m_library;
		FT_Face m_face;
		std::vector<uint8_t> m_faceData;
		std::unordered_map<UnicodeString::char_type, CharacterOpenGL> m_chars;
};