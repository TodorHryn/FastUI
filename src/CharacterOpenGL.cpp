#include "CharacterOpenGL.h"
#include <stdexcept>

namespace fastui
{
	CharacterOpenGL::CharacterOpenGL()
		: valid(false)
	{
	}

	CharacterOpenGL::CharacterOpenGL(FT_Face& face, FT_Int32 c)
		: valid(true)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			throw std::runtime_error("Failed to load char");

		m_width = face->glyph->bitmap.width;
		m_height = face->glyph->bitmap.rows;
		m_bearingX = face->glyph->bitmap_left;
		m_bearingY = face->glyph->bitmap_top;
		m_advance = face->glyph->advance.x;

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			m_width,
			m_height,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	CharacterOpenGL::CharacterOpenGL(CharacterOpenGL&& o)
	{
		*this = o;
		o.valid = false;
	}

	CharacterOpenGL& CharacterOpenGL::operator=(CharacterOpenGL&& o)
	{
		*this = o;
		o.valid = false;
		return *this;
	}

	CharacterOpenGL::~CharacterOpenGL()
	{
		if (valid)
			glDeleteTextures(1, &m_texture);
	}
};
