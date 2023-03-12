#pragma once
#include <ft2build.h>
#include <freetype\freetype.h>
#include <string>
#include <glad\glad.h>

struct CharacterOpenGL
{
	CharacterOpenGL();
	CharacterOpenGL(FT_Face &face, FT_Int32 c);

	int32_t m_size = 48;
	GLuint m_texture;
	int32_t m_width, m_height;
	int32_t m_bearingX, m_bearingY;
	int32_t m_advance;
};