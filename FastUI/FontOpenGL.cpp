#include "FontOpenGL.h"
#include <freetype\freetype.h>
#include <freetype\ftglyph.h>
#include <freetype\ftoutln.h>
#include <freetype\fttrigon.h>

FT_Library *FontOpenGL::m_library = nullptr;

FontOpenGL::FontOpenGL()
{
	if (m_library == nullptr)
	{
		m_library = new FT_Library;
		if (FT_Init_FreeType(m_library))
			throw std::exception("Failed to initialize freetype");
	}
}

FontOpenGL::FontOpenGL(const std::string &path)
	: FontOpenGL()
{
	load(path);
}

FontOpenGL::~FontOpenGL()
{
	FT_Done_Face(m_face);
}

void FontOpenGL::load(const std::string &path)
{
	if (FT_New_Face(*m_library, path.c_str(), 0, &m_face))
		throw std::exception("Failed to load font");
	FT_Set_Pixel_Sizes(m_face, 0, 48);
}

void FontOpenGL::load(const std::vector<uint8_t>& data)
{
	m_faceData = data;
	if (FT_New_Memory_Face(*m_library, m_faceData.data(), m_faceData.size(), 0, &m_face))
		throw std::exception("Failed to load font");
	FT_Set_Pixel_Sizes(m_face, 0, 48);
}

CharacterOpenGL& FontOpenGL::get(UChar32 ch)
{
	if (m_chars.count(ch) == 0)
		m_chars.insert(std::make_pair(ch, CharacterOpenGL(m_face, ch)));
	return m_chars[ch];
}
