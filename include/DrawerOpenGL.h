#pragma once
#include "Drawer.h"
#include "FontOpenGL.h"

class GLFWwindow;

namespace fastui
{	
	class DrawerOpenGL : public Drawer
	{
	public:
		DrawerOpenGL(int32_t width, int32_t height);
		virtual ~DrawerOpenGL();

		virtual void execute() override;
		virtual void render() override;
		virtual void setTitle(const UnicodeString& title) override;
		virtual void drawRectangle(int32_t x, int32_t y, int32_t width, int32_t height, Color color) override;
		virtual void drawRoundedRectangle(int32_t x, int32_t y, int32_t width, int32_t height, Color color) override;
		virtual void drawShadowBorder(int32_t x, int32_t y, int32_t width, int32_t height, int32_t thickness, Color color) override;
		virtual void drawText(int32_t x, int32_t baselineY, int32_t size, Color color, const UnicodeString& text, int32_t cursorPos) override;
		virtual void drawText(int32_t x, int32_t baselineY, int32_t width, int32_t height, int32_t size, Color color, const UnicodeString& text, int32_t cursorPos) override;
		virtual void drawImage(int32_t x, int32_t y, int32_t width, int32_t height, const Image& img) override;
		virtual TextSize measureText(int32_t size, const UnicodeString& text) override;
		virtual TextSize measureText(int32_t width, int32_t size, const UnicodeString& text) override;

		virtual int32_t getTimeMs() const override;

	protected:
		CharacterOpenGL& getChar(UnicodeString::char_type ch);

		void enableScissor();
		void disableScissor();
		void drawRoundedCorner(int32_t x, int32_t y, int32_t width, int32_t height, Color color, float angle);
		void drawChar(const CharacterOpenGL& c, int32_t x, int32_t y, int32_t size, Color color);
		float glx(int32_t x, int32_t width);
		float gly(int32_t y, int32_t height);
		float glwidth(int32_t width);
		float glheight(int32_t height);

		GLFWwindow* m_window;
		unsigned int m_rectVAO, m_rectVBO;
		unsigned int m_cornerVAO, m_cornerVBO;
		ShaderProgram m_rectShader, m_charShader, m_imageShader;
		std::vector<FontOpenGL> m_fonts;
		std::pair<std::shared_ptr<View>, bool> m_viewAtMousePos; //view; overlay at mouse pos?
	};
};