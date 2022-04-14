#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "ShaderProgram.h"
#include "Events.h"

class View;

class Drawer : public std::enable_shared_from_this<Drawer>
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

		void onCharInput(wchar_t ch);
		void onKeyboardEvent(const KeyboardEvent &ev);
		void onMouseEvent(const MouseEvent &ev);
		void onScroll(int32_t x, int32_t y, double xoffset, double yoffset);

		virtual void execute() = 0;
		virtual void render() = 0;
		virtual void drawRectange(int32_t x, int32_t y, int32_t width, int32_t height, Color color) = 0;
		virtual void drawText(int32_t x, int32_t y, int32_t size, Color color, const std::wstring &text, int32_t cursorPos = -1) = 0;
		virtual void drawText(int32_t x, int32_t y, int32_t width, int32_t height, int32_t size, Color color, const std::wstring &text, int32_t cursorPos = -1) = 0;
		virtual std::pair<int32_t, int32_t> measureText(int32_t size, const std::wstring &text) = 0;
		virtual std::pair<int32_t, int32_t> measureText(int32_t width, int32_t size, const std::wstring &text) = 0;

		virtual int32_t getTimeMs() const = 0;

		void translate(int32_t x, int32_t y);
		void translateTo(int32_t x, int32_t y);
		void focus(std::shared_ptr<View> view);
		void setRoot(std::shared_ptr<View> view);
		void setState(const State &state);
		void setSize(int32_t width, int32_t height);
		void setScissor(int32_t x, int32_t y, int32_t width, int32_t height);

		const State& state() const;
		int32_t width() const;
		int32_t height() const;
		bool isFocused(std::shared_ptr<View> view);

	protected:
		int32_t m_width, m_height;
		State m_state;
		std::shared_ptr<View> m_focusedView;
		std::shared_ptr<View> m_root;
};