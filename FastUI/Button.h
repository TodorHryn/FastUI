#pragma once

#include "View.h"

class Button : public View
{
	public:
		Button(SizePolitics width, SizePolitics height);
		virtual ~Button();

		virtual void draw(Drawer &drawer, int32_t width, int32_t height) override;
};