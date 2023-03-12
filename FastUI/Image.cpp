#include "Image.h"

namespace fastui
{
	Image::Image(SizePolitics width, SizePolitics height)
		: View(width, height)
	{
	}

	Image::~Image()
	{
	}

	void Image::draw(int32_t width, int32_t height)
	{
		Drawer::State state = m_drawer->state();

		m_drawer->drawImage(m_paddingX, m_paddingY, width - m_paddingX * 2, height - m_paddingY * 2, m_image);

		m_drawer->setState(state);
	}

	int32_t Image::getMinWidth(int32_t expectedHeight) const
	{
		return m_image.m_width + m_paddingX * 2;
	}

	int32_t Image::getMinHeight(int32_t expectedWidth) const
	{
		return m_image.m_height + m_paddingY * 2;
	}
};
