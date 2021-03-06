#pragma once
#include "Drawer.h"
#include "FontOpenGL.h"

class DrawerOpenGL : public Drawer
{
	public:
		DrawerOpenGL(int32_t width, int32_t height);
		virtual ~DrawerOpenGL();

		virtual void execute() override;
		virtual void render() override;
		virtual void drawRectange(int32_t x, int32_t y, int32_t width, int32_t height, Color color) override;
		virtual void drawText(int32_t x, int32_t y, int32_t size, Color color, const std::wstring &text, int32_t cursorPos) override;
		virtual void drawText(int32_t x, int32_t y, int32_t width, int32_t height, int32_t size, Color color, const std::wstring &text, int32_t cursorPos) override;
		virtual std::pair<int32_t, int32_t> DrawerOpenGL::measureText(int32_t size, const std::wstring &text) override;
		virtual std::pair<int32_t, int32_t> DrawerOpenGL::measureText(int32_t width, int32_t size, const std::wstring &text) override;

		virtual int32_t getTimeMs() const override;

	protected:
		void enableScissor();
		void disableScissor();
		void drawChar(const CharacterOpenGL &c, int32_t x, int32_t y, int32_t size, Color color);
		float glx(int32_t x, int32_t width);
		float gly(int32_t y, int32_t height);
		float glwidth(int32_t width);
		float glheight(int32_t height);

		GLFWwindow *m_window;
		unsigned int m_rectVAO, m_rectVBO;
		ShaderProgram m_rectShader, m_charShader;
		FontOpenGL m_font;
};