#pragma once
#include <cstdint>
#include <string>
#include <vector>

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
		};

		Drawer(int32_t width, int32_t height);
		virtual ~Drawer();

		void clear();
		void translate(int32_t x, int32_t y);
		void drawRectange(int32_t x, int32_t y, int32_t width, int32_t height, Color color);
		void drawText(int32_t x, int32_t y, int32_t size, Color color, const std::wstring &text);
		State state() const;
		void setState(const State &state);
		unsigned char* get();

	private:
		int32_t m_width, m_height;
		State m_state;
		unsigned char *m_buffer;
};