#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "ShaderProgram.h"
#include "Font.h"

class Drawer
{
	public:
		struct Color {
			uint8_t R, G, B, A;

			Color(uint8_t r = 0x00, uint8_t g = 0x00, uint8_t b = 0x00, uint8_t a = 0xFF) : R(r), G(g), B(b), A(a) {}
		};
		struct State {
			int32_t m_translate_x = 0;
			int32_t m_translate_y = 0;
			int32_t m_scissorX = 0, m_scissorY = 0;
			int32_t m_scissorWidth = 0, m_scissorHeight = 0;
		};

		Drawer(int32_t width, int32_t height);
		virtual ~Drawer();

		void clear();
		void translate(int32_t x, int32_t y);
		void setScissor(int32_t x, int32_t y, int32_t width, int32_t height);
		void drawRectange(int32_t x, int32_t y, int32_t width, int32_t height, Color color);

		void drawText(int32_t x, int32_t y, int32_t size, Color color, const std::wstring &text);
		void drawText(int32_t x, int32_t y, int32_t width, int32_t height, int32_t size, Color color, const std::wstring &text);
		std::pair<int32_t, int32_t> measureText(int32_t size, const std::wstring &text);

		const State& state() const;
		void setState(const State &state);
		void setSize(int32_t width, int32_t height);
		int32_t width() const;
		int32_t height() const;

	private:
		void drawChar(const Character &c, int32_t x, int32_t y, int32_t size, Color color);

		int32_t m_width, m_height;
		State m_state;
		unsigned int m_rectVAO, m_rectVBO;
		ShaderProgram m_rectShader, m_charShader;
		Font m_font;
};