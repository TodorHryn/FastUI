#pragma once
#include <ft2build.h>
#include <freetype/freetype.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "CharacterOpenGL.h"
#include "UnicodeString.h"

namespace fastui
{
	class FontOpenGL
	{
	public:
		FontOpenGL();
		FontOpenGL(const std::string& path);
		FontOpenGL(const FontOpenGL&) = delete;
		FontOpenGL(FontOpenGL&&) = default;
		FontOpenGL& operator=(FontOpenGL&&) = default;
		virtual ~FontOpenGL();

		virtual void load(const std::string& path);
		virtual void load(const std::vector<uint8_t>& data);

		virtual CharacterOpenGL& get(UnicodeString::char_type ch);
		virtual bool contains(UnicodeString::char_type ch) const;

	private:
		static FT_Library* m_library;
		std::shared_ptr<FT_Face> m_face;
		std::vector<uint8_t> m_faceData;
		std::unordered_map<UnicodeString::char_type, CharacterOpenGL> m_chars;
	};
};