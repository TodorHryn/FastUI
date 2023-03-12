#include "Font.h"
#include <freetype\freetype.h>
#include <freetype\ftglyph.h>
#include <freetype\ftoutln.h>
#include <freetype\fttrigon.h>

FT_Library *Font::m_library = nullptr;

Font::Font(const std::wstring &wpath)
{
	if (m_library == nullptr)
	{
		m_library = new FT_Library;
		if (FT_Init_FreeType(m_library))
			throw std::exception("Failed to initialize freetype");
	}

	std::string path(wpath.begin(), wpath.end());
	if (FT_New_Face(*m_library, path.c_str(), 0, &m_face))
		throw std::exception("Failed to load font");

	FT_Set_Pixel_Sizes(m_face, 0, 24); //48
}

Font::~Font()
{
	FT_Done_Face(m_face);
}

Character& Font::get(wchar_t ch)
{
	if (m_chars.count(ch) == 0)
		m_chars.insert(std::make_pair(ch, Character(m_face, ch)));
	return m_chars[ch];
}

