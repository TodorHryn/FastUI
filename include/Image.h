#pragma once

#include "View.h"
#include "ImageOpenGL.h"

namespace fastui
{
	class Image : public View
	{
	public:
		Image(SizePolitics width = SizePolitics::WRAP_CONTENT, SizePolitics height = SizePolitics::WRAP_CONTENT);
		virtual ~Image();

		virtual void draw(int32_t width, int32_t height) override;

		virtual int32_t getMinWidth(int32_t expectedHeight) const override;
		virtual int32_t getMinHeight(int32_t expectedWidth) const override;

		ImageOpenGL m_image;
	};
};