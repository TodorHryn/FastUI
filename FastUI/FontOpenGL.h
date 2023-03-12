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
		FontOpenGL(const std::string &path);
		FontOpenGL(const FontOpenGL&) = delete;
		virtual ~FontOpenGL();

		virtual void load(const std::string &path);
		virtual void load(const std::vector<uint8_t> &data);

		virtual CharacterOpenGL& get(UChar32 ch);

		void operator=(const FontOpenGL&) = delete;

	private:
		static FT_Library *m_library;
		FT_Face m_face;
		std::vector<uint8_t> m_faceData;
		std::unordered_map<UChar32, CharacterOpenGL> m_chars;
};