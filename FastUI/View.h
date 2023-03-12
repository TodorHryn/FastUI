#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <unicode/unistr.h>
#include "Drawer.h"
#include "Events.h"

class View : public std::enable_shared_from_this<View>
{
	public:
		enum class SizePolitics {
			MATCH_PARENT,
			WRAP_CONTENT
		};

		View(SizePolitics width, SizePolitics height);
		virtual ~View();

		virtual void setOnMouseEvent(std::function<bool(const MouseEvent &ev)> handler);
		virtual void setDrawer(std::shared_ptr<Drawer> drawer);

		virtual bool onScroll(int32_t x, int32_t y, double xoffset, double yoffset);
		virtual bool onMouseEventOverlay(const MouseEvent& ev);
		virtual bool onMouseEvent(const MouseEvent &ev);
		virtual void onKeyboardEvent(const KeyboardEvent &ev);
		virtual void onCharInput(UChar32 ch);

		virtual void draw(int32_t width, int32_t height) = 0;
		virtual void drawOverlay(int32_t width, int32_t height);
		virtual int32_t getMinWidth(int32_t expectedHeight) const;
		virtual int32_t getMinHeight(int32_t expectedWidth) const;
		const SizePolitics& getWidthPolitics() const;
		const SizePolitics& getHeightPolitics() const;

		Drawer::Color m_backgroundColor;

		int32_t m_paddingX, m_paddingY;
	protected:
		SizePolitics m_width, m_height;
		std::function<bool(const MouseEvent &ev)> m_onMouseEvent;
		std::shared_ptr<Drawer> m_drawer;
};