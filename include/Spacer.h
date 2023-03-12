#pragma once
#include "View.h"

namespace fastui
{
	class Spacer : public View
	{
	public:
		Spacer();
		Spacer(int32_t minWidth, int32_t minHeight);
		virtual ~Spacer();

		virtual void draw(int32_t width, int32_t height) override;

		virtual int32_t getMinWidth(int32_t expectedHeight) const override;
		virtual int32_t getMinHeight(int32_t expectedWidth) const override;

	protected:
		int32_t m_minWidth, m_minHeight;
	};
};