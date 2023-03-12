#include "Spacer.h"

namespace fastui
{
	Spacer::Spacer()
		: View(SizePolitics::MATCH_PARENT, SizePolitics::MATCH_PARENT)
		, m_minWidth(0)
		, m_minHeight(0)
	{
	}

	Spacer::Spacer(int32_t minWidth, int32_t minHeight)
		: View(SizePolitics::WRAP_CONTENT, SizePolitics::WRAP_CONTENT)
		, m_minWidth(minWidth)
		, m_minHeight(minHeight)
	{
	}

	Spacer::~Spacer()
	{
	}

	void Spacer::draw(int32_t width, int32_t height)
	{
	}

	int32_t Spacer::getMinWidth(int32_t expectedHeight) const
	{
		return m_minWidth;
	}

	int32_t Spacer::getMinHeight(int32_t expectedWidth) const
	{
		return m_minHeight;
	}
};
