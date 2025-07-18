#include "FontOpenGL.h"
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <stdexcept>

namespace fastui
{
	FT_Library* FontOpenGL::m_library = nullptr;

	FontOpenGL::FontOpenGL()
		: m_face(std::make_shared<FT_Face>())
	{
		if (m_library == nullptr)
		{
			m_library = new FT_Library;
			if (FT_Init_FreeType(m_library))
				throw std::runtime_error("Failed to initialize freetype");
		}
	}

	FontOpenGL::FontOpenGL(const std::string& path)
		: FontOpenGL()
	{
		load(path);
	}

	FontOpenGL::~FontOpenGL()
	{
		if (m_face)
			FT_Done_Face(*m_face);
	}

	void FontOpenGL::load(const std::string& path)
	{
		if (FT_New_Face(*m_library, path.c_str(), 0, m_face.get()))
			throw std::runtime_error("Failed to load font");
		FT_Set_Pixel_Sizes(*m_face, 0, 24); //48
	}

	void FontOpenGL::load(const std::vector<uint8_t>& data)
	{
		m_faceData = data;
		if (FT_New_Memory_Face(*m_library, m_faceData.data(), static_cast<FT_Long>(m_faceData.size()), 0, m_face.get()))
			throw std::runtime_error("Failed to load font");
		FT_Set_Pixel_Sizes(*m_face, 0, 24); //48
	}

	CharacterOpenGL& FontOpenGL::get(UnicodeString::char_type ch)
	{
		if (m_chars.count(ch) == 0)
		{
			CharacterOpenGL img(*m_face, ch);
			m_chars.emplace(ch, std::move(img));
		}
		return m_chars[ch];
	}
	bool FontOpenGL::contains(UnicodeString::char_type ch) const
	{
		return FT_Get_Char_Index(*m_face, ch) != 0;
	}
};
