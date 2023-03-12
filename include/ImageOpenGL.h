#pragma once
#include <cstdint>
#include <glad\glad.h>
#include "Drawer.h"

namespace fastui
{
	struct ImageOpenGL : public Drawer::Image
	{
		ImageOpenGL();
		ImageOpenGL(const ImageOpenGL&) = delete;
		ImageOpenGL(ImageOpenGL&&) noexcept;
		ImageOpenGL& operator=(ImageOpenGL&&) noexcept;
		virtual ~ImageOpenGL();

		void load(const std::string& path);

		int32_t m_width, m_height;
		GLuint m_texture;

	private:
		void load(uint8_t* glData);
		ImageOpenGL& operator=(const ImageOpenGL&) = default;

		bool valid = false;
	};
};
